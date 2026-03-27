#pragma once

#include <filesystem>
#include <string_view>

namespace wcounter {

class FileReader {
public:
    FileReader(const std::filesystem::path&);
    std::string_view next();
    ~FileReader();
private:
    int m_file;
    const char* m_data;
    size_t m_size{0};
    size_t m_pos{0};
};

}