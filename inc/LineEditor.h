//
// Created by Kaitlyn Archambault on 2023-09-16.
//

#ifndef ASSIGNMENT1_LINEEDITOR_H
#define ASSIGNMENT1_LINEEDITOR_H

#include"../inc/LinkedList.h"

class LineEditor : LinkedList {
public:
    LineEditor();
    virtual ~LineEditor();
    //explicit LineEditor(std::stringstream stream_contents);
    int getLastLine() const;
    int getWorkingLine() const;
    std::string getLine(int line_num);
    bool isEditing();
    static bool isValidOperation(const std::string& input);
    void initOperations(const std::string& input);
    void initContents(std::stringstream stream_contents);
    void execute(const std::string& input);
    void printLine(int line_num);

private:
    enum Operations {
        Operation_Invalid,
        Insert,
        Revise,
        Delete,
        List,
        Exit
    };
    static Operations resolveOperations(const std::string& input);
    Operations operation;
    std::string user_input;
    int range_start;
    int range_end;
    int working_line;
};


#endif //ASSIGNMENT1_LINEEDITOR_H
