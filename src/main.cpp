#include <format>
#include <iostream>
#include <stdexcept>
#include "file_reader.hpp"

int main(int argc, char* argv[]) {
    try{
        if (argc < 2) {
            throw std::invalid_argument("Invalid input");
        }
        auto reader = wcounter::FileReader(std::filesystem::path(argv[1]));
        for(;;) {
            auto sv = reader.next();
            if(sv.empty()) {
                break;
            }
            std::cout << std::format("{} : {}", sv, sv.size()) << std::endl;
        }
        return EXIT_SUCCESS;
    } catch(const std::exception& ex) {
        std::cerr << std::format("{}", ex.what()) << std::endl;
        return EXIT_FAILURE;
    }
}