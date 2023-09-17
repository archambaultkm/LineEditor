//
// Created by Kaitlyn Archambault on 2023-09-16.
//

#include <iostream>
#include <utility>
#include "../inc/LineEditor.h"


LineEditor::LineEditor() {
    size = 0;
    head = nullptr;
    tail = nullptr;
}

//used for range argument validation
int LineEditor::getSize() {
    return size;
}

//TODO this might be a nice feature but not needed for assignment
//iterate through lines and search for query in data, return any lines it's on
//(number + text), show query in another colour?
void LineEditor::search(std::string query) {

}

//insert a new line into the linked list, and update line numbers on existing nodes
void LineEditor::insertLine(int line_num, std::string text) {
    if (line_num > 0 && line_num <= size + 1) {
        Line * temp = new Line();
        temp->data = std::move(text);
        //If the text is being inserted at the end of the document
        if (line_num == size + 1) {
            temp->position = size + 1;
            temp->link = nullptr;
            //If this is the first line being inserted
            if (size == 0) {
                head = temp;
                tail = temp;
            } else {
                //link the current tail to the new last line,
                //and change the tail property to point to the new last line
                tail->link = temp;
                if (size == 1) head->link = temp;
                tail = temp;
            }
        //inserting text in the beginning or middle of document
        } else {
            //insert the new line at the position specified
            temp->position = line_num;
            Line * curr = head;
            Line * prev = nullptr;

            //special case for inserting on first line
            if (line_num == 1) {
                head = temp;
                temp->link = curr;
            } else {
                while ( curr != nullptr && curr->position != line_num) {
                    prev = curr;
                    curr = curr->link;
                }
                //new line gets inserted between the previous and current line
                prev->link = temp;
                temp->link = curr;
            }
        }
        //update the size property of the line editor, and update each line's position for printing
        size++;
        updateLineNumbers();
    }
}

//iterate through the document to find desired line and replace its data with given text
void LineEditor::editLine(int position, std::string new_text) {
    if (position > 0 && position <= size) {
        Line * curr = head;
        while (curr != nullptr && curr->position != position) curr = curr->link;
        curr->data = std::move(new_text);
    }
}

//iterate through document to find the line at the given position, and safely delete it
void LineEditor::deleteLine(int line_num) {
    if (line_num > 0 && line_num <= size) {
        Line * curr = head;
        Line * prev = nullptr;

        //keep looping through until reaching the desired position
        while (curr != nullptr && curr->position != line_num) {
            prev = curr;
            curr = curr->link;
        }
        Line * temp;

        //special cases for first and last lines in document
        if (line_num == 1) {
            temp = head;
            head = head->link;
            delete temp;
            updateLineNumbers();

        } else if (line_num == size) {
            temp = curr;
            tail = prev;
            tail->link = nullptr;
            delete temp;

        //any lines in the middle of the document
        } else {
            if (prev != nullptr && curr != nullptr) prev->link = curr->link;
            delete curr;
            updateLineNumbers();
        }

        //update the size value of the document to reflect the deletion
        size--;
    }
}

std::ostream &operator<<(std::ostream &output, LineEditor document) {
    auto line = document.head;

    //output each line of the document
    while (line != nullptr) {
        std::cout << line->position << "> " << line->data << std::endl;
        line = line->link;
    }

    return output;
}

//update position property on all lines, used after inserting or deleting lines
void LineEditor::updateLineNumbers() {
    Line * line = head;
    Line * prev = nullptr;

    while (line != nullptr) {
        if (prev == nullptr)
            line->position = 1;
        else
            line->position = prev->position + 1;

        prev = line;
        line = line->link;
    }
}

void LineEditor::printRange(int range_start, int range_end) {
    if (range_start > range_end) {
        int temp = range_end;
        range_end = range_start;
        range_start = temp;
    }

    Line * curr = head;
    //get to the start of the range to print in the list
    while (curr != nullptr && curr->position != range_start) curr = curr->link;

    for (int i=range_start;i<=range_end;i++) {
        std::cout << curr->position << "> " << curr->data << std::endl;
        curr = curr->link;
    }
}

void LineEditor::printLine(int line_num) {
    Line * curr = head;

    //get to the desired line to print
    while (curr != nullptr && curr->position != line_num) curr = curr->link;
    std::cout << curr->position << "> " << curr->data << std::endl;
}

