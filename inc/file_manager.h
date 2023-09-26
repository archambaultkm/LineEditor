/**
 * @author Kaitlyn Archambault
 * @date Sept. 26, 2023
 */
#ifndef ASSIGNMENT1_FILEMANAGER_H
#define ASSIGNMENT1_FILEMANAGER_H
#include <string>
#include <fstream>
#include <sstream>

/**
 * @brief FileManager class with dedicated functions to handle file i/o and streams
 *
 * This is meant to be a general purpose class to use whenever a project requires file i/o.
 * Stringstreams can be both read from and written to provided file paths.
 *
 */
class FileManager {
public:
    FileManager();
    virtual ~FileManager();
    std::stringstream readStreamFromFile(std::string file_name);
    void writeStreamToFile(const std::string& file_name, std::stringstream ss);

private:
    std::ifstream m_ifs;
    std::ofstream m_ofs;

};
#endif //ASSIGNMENT1_FILEMANAGER_H