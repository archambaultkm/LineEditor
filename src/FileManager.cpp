//
// Created by Kaitlyn Archambault on 2023-09-21.
//

#include "../inc/FileManager.h"
#include <iostream>
#include <utility>


FileManager::FileManager(std::string file_name) {
    this->file_name = std::move(file_name);
}

std::stringstream FileManager::readStreamFromFile() {
    std::string file_line;

    try {
        ifs.open(file_name, std::fstream::in);

        if (ifs.fail()) {
            std::cout << "Could not open file.";
            //return 1;
        }

        //take each line from file (even if empty) and insert into line editor
        while (getline(ifs, file_line)) {
            file_contents_stream << file_line << std::endl;
        }

    }  catch (std::ifstream::failure &e) {
        std::cout << "Exception opening/closing/reading file" << std::endl;
    }

    ifs.close();

    return std::move(file_contents_stream);
}

void FileManager::writeStreamToFile(std::stringstream ss) {
    std::string file_line;

    try {
        ofs.open(file_name, std::fstream::trunc );

        if (ofs.fail()) {
            std::cout << "Could not open file.";
        }

        while (getline(ss, file_line)) {
            ofs << file_line << std::endl;
        }

    }  catch (std::ofstream::failure &e) {
        std::cout << "Exception opening/closing/reading file" << std::endl;
    }

    ofs.close();
}
