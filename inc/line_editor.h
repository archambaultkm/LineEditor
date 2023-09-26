//
// Created by Kaitlyn Archambault on 2023-09-16.
//

#ifndef ASSIGNMENT1_LINE_EDITOR_H
#define ASSIGNMENT1_LINE_EDITOR_H

#include"../inc/linked_list.h"
#include <fstream>

class LineEditor : LinkedList {
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
