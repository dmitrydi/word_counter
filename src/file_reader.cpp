#include "file_reader.hpp"

#include <cctype>
#include <cerrno>
#include <cstring>
#include <format>
#include <stdexcept>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>

namespace wcounter {

FileReader::FileReader(const std::filesystem::path& p){
    m_file = open(p.c_str(), O_RDONLY);
    if(m_file == -1) {
        throw std::runtime_error(std::format("Could not read {}: {}", p.string(), std::strerror(errno)));
    }
    m_size = lseek(m_file, 0, SEEK_END);
    m_data = (const char*)mmap(nullptr, m_size, PROT_READ, MAP_PRIVATE, m_file, 0);
    madvise((void*)m_data, m_size, MADV_SEQUENTIAL);
}


std::string_view FileReader::next(){
    m_pos = std::find_if(m_data + m_pos, m_data + m_size, [](char c) { return std::isalpha(c); }) - m_data;
    if(m_pos >= m_size) return {};
    auto start = m_pos;
    m_pos = std::find_if(m_data + m_pos, m_data + m_size, [](char c) { return !std::isalpha(c); }) - m_data;
    return {m_data+start, m_pos - start};
}

FileReader::~FileReader(){
    munmap((void*)m_data, m_size);
    close(m_file);
}

}