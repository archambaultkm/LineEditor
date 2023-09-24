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
    FileManager();
    virtual ~FileManager();
    std::stringstream readStreamFromFile();
    void writeStreamToFile(std::stringstream ss);
    void setFileName(std::string file_name);

private:
    std::string file_name;
    std::ifstream ifs;
    std::ofstream ofs;
    std::stringstream file_contents_stream;
};


#endif //ASSIGNMENT1_FILEMANAGER_H
