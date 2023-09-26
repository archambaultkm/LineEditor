/**
 * @brief Program to open, edit and save a text file
 * using Linked List data structure
 *
 * @author Kaitlyn Archambault
 * @date Sept. 26, 2023
 */

#include <iostream>
#include <regex>
#include "../inc/line_editor.h"

using namespace std;

bool isValidFilePath(const string& provided_file_path);

int main(int argc, char *argv[]) {
    LineEditor line_editor;
    string user_input, file_name;

    // expected argument format is "./edit file_path.txt"
    if (argc == 3 && strcmp(argv[1], "edit") == 0) {
        if (!isValidFilePath(argv[2])) {
            cout << "Must enter a valid .txt format file name" << endl;
            return 1;
        }
    } else {
        cout << "Invalid arguments" << endl;
        return 1;
    }

    // load/create the file given as command line argument
    file_name = argv[2];
    line_editor.readFromFile(file_name);

    cout << line_editor;

    do {
        // take user input and let the line editor figure out what to do with it
        cout << line_editor.getWorkingLine() << "> " ;
        getline(cin, user_input);
        line_editor.execute(user_input);

    } while (line_editor.isEditing());

    // when done, save the file.
    line_editor.writeToFile(file_name);
    std::cout << "Contents saved to " << file_name << endl;
    cout << "Exiting application..." << endl;

    return 0;
}

/**
 * Validate provided file path with regex
 *
 * @param provided_file_path file path input by user
 *
 * @returns bool
 * @note Expected file path format is "(/path/)file_name.txt"
 */
bool isValidFilePath(const string& provided_file_path) {
    regex valid_file_path(R"(^(..)?\/?([a-zA-Z0-9_-]+[\/])?([a-zA-Z0-9_-])+\.txt$)");

    return regex_match(provided_file_path, valid_file_path);
}