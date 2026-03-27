#pragma once

#include <cctype>
#include <cerrno>
#include <concepts>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <string_view>
#include <sys/mman.h>
#include <unistd.h>
#include <unordered_map>
#include <cstdint>
#include <algorithm>
#include <vector>

namespace wcounter {

inline unsigned char to_lower_ascii(unsigned char c) {
    return (c >= 'A' && c <= 'Z') ? (c | 0x20) : c;
}

struct CaseInsensitiveHash { // FNV-1a
    size_t operator()(std::string_view sv) const noexcept {
        constexpr size_t FNV_OFFSET = 0xcbf29ce484222325ULL;
        constexpr size_t FNV_PRIME = 0x100000001b3ULL;
        
        size_t hash = FNV_OFFSET;
        for (unsigned char c : sv) {
            hash ^= to_lower_ascii(c);
            hash *= FNV_PRIME;
        }
        
        return hash;
    }
};

struct CaseInsensitiveEqual {
    bool operator()(std::string_view a, std::string_view b) const noexcept {
        if (a.size() != b.size()) return false;
        return std::equal(a.begin(), a.end(), b.begin(),
            [](unsigned char ca, unsigned char cb) noexcept {
                return to_lower_ascii(ca) == to_lower_ascii(cb);
            });
    }
};

inline bool case_insensitive_less(std::string_view a, std::string_view b) {
        size_t min_len = std::min(a.size(), b.size());
        for (size_t i = 0; i < min_len; ++i) {
            auto la = to_lower_ascii(a[i]); 
            auto lb = to_lower_ascii(b[i]); 
            if (la != lb) return la < lb;
        }
        return a.size() < b.size();
}

using MapType = std::unordered_map<std::string_view, uint64_t, CaseInsensitiveHash, CaseInsensitiveEqual>;

template<typename T>
concept HasNext = requires(T t) {
    { t.next() } -> std::same_as<std::string_view>;
};

template<HasNext ReaderType>
MapType make_mapping(ReaderType& reader) {
    MapType result;
    for(;;) {
        auto sv = reader.next();
        if(sv.empty()) { break; }
        auto [it,_] = result.try_emplace(sv, 0);
        ++it->second;
    }
    
    return result;
}

inline std::vector<const MapType::value_type*> get_sorted_stats(const MapType& m) {
    std::vector<const MapType::value_type*> entries;
    entries.reserve(m.size());

    for(const auto& e: m) {
        entries.push_back(&e);
    }

    std::sort(entries.begin(), entries.end(), 
        [](const MapType::value_type* a, const MapType::value_type* b) {
            if (a->second != b->second) {
                return a->second > b->second;
            }
            return case_insensitive_less(a->first, b->first);
        });

    return entries;
}


}