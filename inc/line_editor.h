//
// Created by Kaitlyn Archambault on 2023-09-16.
//

#ifndef ASSIGNMENT1_LINE_EDITOR_H
#define ASSIGNMENT1_LINE_EDITOR_H

#include"../inc/linked_list.h"
#include"../src/linked_list.cpp"
#include <fstream>
#include <string>

/**
 * @brief LineEditor class adds functionality to LinkedList class for parsing user input and file i/o.
 *
 * This class handles the validation of user input, working line logic, and the control flow of operations.
 * The LineEditor class also has its own functions for file i/o so that data can move directly between
 * files and the LinkedList.
 *
 * @note m_iter_start and m_iter_end properties are used to determine how many times an operation should be performed.
 * @note m_working_line is used to show the user what line their cursor is currently on
 */
class LineEditor : LinkedList<std::string> {
public:
    ~LineEditor() override;
    LineEditor();
    int getWorkingLine() const;
    bool isEditing();
    void execute(const std::string& input);
    void readFromFile(const std::string& file_name);
    void writeToFile(const std::string& file_name);
    friend std::ostream& operator<<(std::ostream& output, LineEditor& line_editor);

private:
    enum Operation {
        Not_An_Operation,
        Insert,
        Revise,
        Delete,
        List,
        Exit
    };
    Operation m_operation;
    std::string m_user_input;
    int m_iter_start;
    int m_iter_end;
    int m_working_line;
    std::ifstream m_ifs;
    std::ofstream m_ofs;
    static Operation resolveOperations(const std::string& input);
    static bool isValidOperation(const std::string& input);
    void initOperations(const std::string& input);

};


#endif //ASSIGNMENT1_LINE_EDITOR_H
