cmake_minimum_required(VERSION 3.10)

project(word_counter VERSION 1.0)

set(SRC_FILES
    src/file_reader.cpp
    src/file_reader.hpp
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)