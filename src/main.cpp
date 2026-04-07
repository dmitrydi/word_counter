#include <format>
#include <iostream>
#include <stdexcept>
#include "file_reader.hpp"
#include "file_writer.hpp"
#include "word_counter.hpp"

int main(int argc, char* argv[]) {
    try{
        if (argc < 3) {
            throw std::invalid_argument("Invalid input");
        }
        auto reader = wcounter::FileReader(std::filesystem::path(argv[1]));
        auto writer = wcounter::Writer(std::filesystem::path(argv[2]));
        writer.write(wcounter::get_sorted_stats(wcounter::make_mapping(reader)));
        return EXIT_SUCCESS;
    } catch(const std::exception& ex) {
        std::cerr << std::format("{}", ex.what()) << std::endl;
        return EXIT_FAILURE;
    }
}