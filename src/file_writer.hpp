#pragma once

#include "word_counter.hpp"
#include <filesystem>
#include <format>

namespace wcounter {

class Writer {
public:
    Writer(const std::filesystem::path& p) {
        m_fd = open(p.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
        if(m_fd == -1) {
            throw std::runtime_error(std::format("Could not open output file {}: {}", p.string(), strerror(errno)));
        }
    }
    void write(const std::vector<const MapType::value_type*>& entries) const {
        size_t estimated_size = 0;
        for (const auto* entry : entries) {
            estimated_size += entry->first.size() + 22;
        }

        if (ftruncate(m_fd, estimated_size) == -1) {
            throw std::runtime_error(std::format("Could not truncate file: {}", strerror(errno)));
        }

        char* mapped = (char*)mmap(nullptr, estimated_size, PROT_WRITE, 
                                MAP_SHARED, m_fd, 0);

        if (mapped == MAP_FAILED) {
            throw std::runtime_error(std::format("Could not mmap file: {}", strerror(errno)));
        }

            char* ptr = mapped;
        char num_buffer[32];
    
        for (const auto* entry : entries) {
            int len = snprintf(num_buffer, sizeof(num_buffer), "%zu ", entry->second);
            memcpy(ptr, num_buffer, len);
            ptr += len;
            
            std::string_view word = entry->first;
            for (char c : word) {
                *ptr++ = (c >= 'A' && c <= 'Z') ? (c + 32) : c;
            }
            *ptr++ = '\n';
        }
    
        size_t actual_size = ptr - mapped;
        if(munmap(mapped, estimated_size) == -1) {
            throw std::runtime_error(std::format("Could not munmap file: {}", strerror(errno)));
        }

        if(ftruncate(m_fd, actual_size) == -1) {
            throw std::runtime_error(std::format("Could not do final resize: {}", strerror(errno)));
        }
    }
    ~Writer() {
        close(m_fd);
    }
private:
    int m_fd;
};


}