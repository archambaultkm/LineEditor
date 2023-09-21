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

int LineEditor::getLastLine() const {
    return size + 1;
}

//insert a new line into the linked list, and update line numbers on existing nodes
void LineEditor::insertLine(int line_num, std::string text) {
    if (line_num <= 0 || line_num > size + 1) return;

    Line * temp = new Line();
    temp->data = std::move(text);

    //If the text is being inserted at the end of the document
    if (line_num == size + 1) {
        temp->position = size + 1;
        temp->next = nullptr;

        if (size == 0) {
            //If this is the first line being inserted it is both head and tail
            head = temp;
            tail = temp;
        } else {
            //link the current tail to the new last line, link from the head if this is the second node, update the tail.
            tail->next = temp;
            if (size == 1) head->next = temp;
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
            temp->next = curr;
        } else {
            while ( curr != nullptr && curr->position != line_num) {
                prev = curr;
                curr = curr->next;
            }
            //new line gets inserted between the previous and current line
            prev->next = temp;
            temp->next = curr;
        }
    }
    //update size and line numbers to reflect the addition
    size++;
    updateLineNumbers();
}

//iterate through the document to find desired line and replace its data with given text
void LineEditor::editLine(int line_num, std::string new_text) {
    if (line_num <= 0 || line_num > size) return;

    Line * curr = head;
    while (curr != nullptr && curr->position != line_num) curr = curr->next;
    curr->data = std::move(new_text);
}

//TODO do you actually need this or the listRange functions
//call the delete line function for a given range of lines
void LineEditor::deleteRange(int range_start, int range_end) {
    if (range_start > range_end) {
        int temp = range_end;
        range_end = range_start;
        range_start = temp;
    }

    for (int i=range_start;i<=range_end;i++) {
        deleteLine(i);
    }
}

//iterate through document to find the line at the given position, and safely delete it
void LineEditor::deleteLine(int line_num) {
    if (line_num <= 0 || line_num > size) return;

    Line * curr = head;
    Line * prev = nullptr;
    Line * temp;

    //keep looping through until reaching the desired position
    while (curr != nullptr && curr->position != line_num) {
        prev = curr;
        curr = curr->next;
    }

    //special cases for first and last lines in document
    if (line_num == 1) {
        temp = head;
        head = head->next;
        delete temp;

    } else if (line_num == size) {
        temp = curr;
        tail = prev;
        tail->next = nullptr;
        delete temp;

    //any lines in the middle of the document
    } else {
        if (prev != nullptr && curr != nullptr) prev->next = curr->next;
        delete curr;
    }

    //update the size value of the document to reflect the deletion
    updateLineNumbers();
    size--;
}

std::ostream &operator<<(std::ostream &output, LineEditor line_editor) {
    auto line = line_editor.head;

    //output each line of the document
    while (line != nullptr) {
        std::cout << line->position << "> " << line->data << std::endl;
        line = line->next;
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
        line = line->next;
    }
}

void LineEditor::printRange(int range_start, int range_end) {
    //rearrange start+end so that the range is positive
    if (range_start > range_end) {
        int temp = range_end;
        range_end = range_start;
        range_start = temp;
    }

    for (int i=range_start;i<=range_end;i++) {
        printLine(i);
    }
}

void LineEditor::printLine(int line_num) {
    Line * curr = head;

    //get to the desired line to print
    while (curr != nullptr && curr->position != line_num) curr = curr->next;
    std::cout << curr->position << "> " << curr->data << std::endl;
}

