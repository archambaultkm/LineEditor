//
// Created by Kaitlyn Archambault on 2023-09-16.
//

#ifndef ASSIGNMENT1_LINEEDITOR_H
#define ASSIGNMENT1_LINEEDITOR_H

#include"../inc/LinkedList.h"
#include <fstream>

class LineEditor : LinkedList {
private:
    enum Operation {
        Not_An_Operation,
        Insert,
        Revise,
        Delete,
        List,
        Exit
    };
    static Operation resolveOperations(const std::string& input);
    Operation operation;
    std::string user_input;
    std::ifstream ifs;
    std::ofstream ofs;
    int range_start;
    int range_end;
    int working_line;

public:
    ~LineEditor() override;
    LineEditor();
    int getSize() const;
    int getWorkingLine() const;
    bool isEditing();
    void readFromFile(const std::string& file_name);
    void writeToFile(const std::string& file_name);
    static bool isValidOperation(const std::string& input);
    void initOperations(const std::string& input);
    void execute(const std::string& input);
    void printLine(int line_num);
};


#endif //ASSIGNMENT1_LINEEDITOR_H
