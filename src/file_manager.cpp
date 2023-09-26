/**
 * @author Kaitlyn Archambault
 * @date Sept. 26, 2023
 */
#include "../inc/file_manager.h"
#include <iostream>
#include <utility>


FileManager::FileManager() = default;
FileManager::~FileManager() = default;

/**
 * Reads a stream from the given file name
 *
 * @param file_name path to file
 * @returns stream containing the data of the read file
 */
std::stringstream FileManager::readStreamFromFile(std::string file_name) {
    std::string file_line;
    std::stringstream file_contents;

    try {
        m_ifs.open(file_name, std::fstream::in);
        if (m_ifs.fail()) {
            std::cout << "Could not open file.";
            //return 1;
        }
        //take each line from file (even if empty) and insert into line editor
        while (getline(m_ifs, file_line)) {
            file_contents << file_line << std::endl;
        }
    }  catch (std::ifstream::failure &e) {
        std::cout << "Exception opening/closing/reading file" << std::endl;
    }
    m_ifs.close();

    return std::move(file_contents);
}

/**
 * Writes the provided stream to the provided file name
 *
 * @param file_name path to file
 * @param ss stream containing the data to be written to the file
 */
void FileManager::writeStreamToFile(const std::string& file_name, std::stringstream ss) {
    std::string file_line;

    try {
        m_ofs.open(file_name, std::fstream::trunc );
        if (m_ofs.fail()) {
            std::cout << "Could not open file.";
        }
        while (getline(ss, file_line)) {
            m_ofs << file_line << std::endl;
        }
    }  catch (std::ofstream::failure &e) {
        std::cout << "Exception opening/closing/reading file" << std::endl;
    }

    m_ofs.close();
}
