cmake_minimum_required(VERSION 3.17)
project(Assignment1)

set(CMAKE_CXX_STANDARD 14)

add_executable(assignment1
        src/main.cpp
        src/LineEditor.cpp
        inc/LineEditor.h
        src/FileManager.cpp
        inc/FileManager.h
        src/LinkedList.cpp
        inc/LinkedList.h
)
