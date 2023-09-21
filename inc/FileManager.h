//
// Created by Kaitlyn Archambault on 2023-09-21.
//

#ifndef ASSIGNMENT1_FILEMANAGER_H
#define ASSIGNMENT1_FILEMANAGER_H

#include <string>
#include <fstream>
#include <sstream>

class FileManager {
public:
    explicit FileManager(std::string file_name);
    std::stringstream getFileContents();
    int getFileLines() const;
    void readStreamFromFile();
    void writeStreamToFile(std::stringstream ss);

private:
    std::string file_name;
    int file_lines;
    std::ifstream ifs;
    std::ofstream ofs;
    std::stringstream file_contents_stream;
};


#endif //ASSIGNMENT1_FILEMANAGER_H
