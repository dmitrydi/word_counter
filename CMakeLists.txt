cmake_minimum_required(VERSION 3.10)

project(word_counter VERSION 1.0)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(SRC_FILES
    src/file_reader.hpp
    src/word_counter.hpp
    src/file_writer.hpp
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)

target_compile_options(${PROJECT_NAME} PRIVATE 
    -O2
    -march=native
    -funroll-loops
    -flto
)

target_link_options(${PROJECT_NAME} PRIVATE -flto)
