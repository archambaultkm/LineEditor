cmake_minimum_required(VERSION 3.17)
project(Assignment1)

set(CMAKE_CXX_STANDARD 14)

add_executable(assignment1
        src/main.cpp
        src/line_editor.cpp
        inc/line_editor.h
        src/linked_list.cpp
        inc/linked_list.h
)
