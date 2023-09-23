//
// Created by Kaitlyn Archambault on 2023-09-16.
//

#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include "../inc/LineEditor.h"


LineEditor::LineEditor() {
    operation = Insert;
    range_start = 0;
    range_end = 0;
    working_line = 1;
}

LineEditor::~LineEditor() = default;

//LineEditor::LineEditor(std::stringstream stream_contents) {
//    head = nullptr;
//    tail = nullptr;
//
//    std::string line;
//    while (getline(stream_contents, line)) {
//        this->insertLine(size+1, line);
//    }
//}

int LineEditor::getLastLine() const {
    return size + 1;
}

std::string LineEditor::getLine(int line_num) {
    return getNodeData(line_num);
}

int LineEditor::getWorkingLine() const {
    return this->working_line;
}

void LineEditor::printLine(int line_num) {
    std::cout << line_num << "> " << this->getNodeData(line_num) << std::endl;
}

bool LineEditor::isEditing() {
    return operation != Exit;
}

void LineEditor::initContents(std::stringstream stream_contents) {
    std::string line;
    while (getline(stream_contents, line)) {
        this->insertNode(size+1, line);
    }

    working_line = size+1;
}

LineEditor::Operations LineEditor::resolveOperations(const std::string& input) {
    if (input == "I") return Insert;
    if (input == "R") return Revise;
    if (input == "D") return Delete;
    if (input == "L") return List;
    if (input == "E") return Exit;

    return Operation_Invalid;
}

bool LineEditor::isValidOperation(const std::string& input) {
    std::regex valid_operation(R"(^(I|R|D|L|E)([[:space:]][0-9]+)?([[:space:]][0-9]+)?$)");
    return (regex_match(input, valid_operation));
}

void LineEditor::initOperations(const std::string& input) {
    range_start = 0, range_end = 0;
    std::vector<std::string> args;
    std::string arg;

    if (isValidOperation(input)) {

        std::istringstream input_stream(input);
        //send stream content to vector for easy parsing
        while (input_stream >> arg) args.push_back(arg);

        operation = resolveOperations(args.at(0));
        if (args.size() >= 2) {
            range_start = stoi(args.at(1));
            if (operation == Insert) working_line = range_start;
        }
        if (args.size() == 3)
            range_end = stoi(args.at(2));

        //make sure the range is positive
        if (range_start > range_end && range_end != 0) {
            int temp = range_end;
            range_end = range_start;
            range_start = temp;
        }
    } else {
        user_input = input;
        operation = Operation_Invalid;
    }
}

void LineEditor::execute(const std::string& input) {

    initOperations(input);

    switch (operation) {
        case Exit:
            //get outta here
            break;

        case Operation_Invalid:
            //If they haven't entered a valid operation, assume they're trying to insert text on the displayed line
            this->insertNode(working_line, user_input);
            working_line++;

            break;

        case Insert:
            //if they've given no arguments other than I, send them above the current working line.
            if (range_start == 0 && working_line != 1)
                working_line--;

            //TODO figure out how to decouple this
            //prompt with working line number and get input
            std::cout << working_line << "> ";
            getline(std::cin, user_input);

            this->insertNode(working_line, user_input);
            working_line++;

            break;

        case Revise:
            //special case for revise with arguments, don't want to affect the working line
            if (range_start != 0) std::cout << range_start << "> ";
            //if they've given no arguments other than R, send them above the current working line.
            if (range_start == 0 && working_line != 1) std::cout << working_line - 1 << "> ";

            getline(std::cin, user_input);
            if (range_start != 0)
                this->editNode(range_start, user_input);
            else
                this->editNode(working_line - 1, user_input);

            break;

        case Delete:
            //delete a range of lines, a specified line, or the most recent line
            if (range_start != 0 && range_end != 0) {
                for (int i=range_start; i<range_end; i++)
                    this->deleteNode(i);
                //TODO this is awful
                working_line = working_line - (range_end-range_start);
            } else if (range_start != 0) {
                this->deleteNode(range_start);
            } else {
                this->deleteNode(working_line - 1);
            }

            //adjust working line if a line before it was deleted
            if (working_line >= range_start) working_line--;

            break;

        case List:
            //print a range of lines, a single line, or the whole document
            if (range_start != 0 && range_end != 0) {
                for (int i=range_start; i<range_end; i++)
                    this->printLine(i);
                working_line = range_end + 1;
            } else if (range_start != 0) {
                this->printLine(range_start);
                working_line = range_start + 1;
            } else {
                for (int i=1; i<size+ 1; i++)
                    this->printLine(i);
                //reset the working line to end of doc
                working_line = size + 1;
            }

            break;

    }//end switch operations
}
