//
// Created by Kaitlyn Archambault on 2023-09-16.
//

#ifndef ASSIGNMENT1_LINEEDITOR_H
#define ASSIGNMENT1_LINEEDITOR_H

#include <string>

class LineEditor {
public:
    LineEditor();
    int getSize() const;
    void updateLineNumbers();
    void insertLine(int line_num, std::string text);
    void editLine(int position, std::string text);
    void deleteRange(int range_start, int range_end);
    void deleteLine(int position);
    void printRange(int range_start, int range_end);
    void printLine(int line_num);
    friend std::ostream& operator<<(std::ostream& output, LineEditor document);

private:
    //nested struct contains individual lines of text + position in doc, functions as linked list node.
    struct Line {
        int position;
        std::string data;
        Line * link;
    };
    int size;
    Line * head;
    Line * tail;
};


#endif //ASSIGNMENT1_LINEEDITOR_H
