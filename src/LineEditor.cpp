//
// Created by Kaitlyn Archambault on 2023-09-16.
//

#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include <regex>
#include "../inc/LineEditor.h"


LineEditor::LineEditor(std::string file_name) {
    operation = Insert;
    range_start = 0;
    range_end = 0;
    working_line = 1;

    //TODO this probably shouldn't be here
    file_manager.setFileName(std::move(file_name));
    std::stringstream file_contents = file_manager.readStreamFromFile();
    this->initContents(std::move(file_contents));
}

LineEditor::~LineEditor() = default;

int LineEditor::getSize() const {
    return size;
}

int LineEditor::getWorkingLine() const {
    return this->working_line;
}

bool LineEditor::isEditing() {
    return operation != Exit;
}

void LineEditor::printLine(int line_num) {
    std::cout << line_num << "> " << this->getNodeData(line_num) << std::endl;
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

    //TODO maybe move to range init function
    //also maybe for range: instead of doing this, take the first line and figure out how many times to iterate your operation from range_end - range_start
    //make sure the range is positive
    if (range_start > range_end && range_end != 0) {
        int temp = range_end;
        range_end = range_start;
        range_start = temp;
    }

    if (range_start == 0) {
        if (operation == List) {
            range_start = 1;
            range_end = size;
            if (range_end == 0) range_end = 1;
            return;
        }
        if (working_line != 1) range_start = working_line - 1;
    }
    if (range_end == 0) {
        range_end = range_start;
    }
}

void LineEditor::execute(const std::string& input) {
    if (!isValidOperation(input)) {
        user_input = input;
        operation = Operation_Invalid;
    } else initOperations(input);

    switch (operation) {
        case Operation_Invalid:
            //If they haven't entered a valid operation, assume they're trying to insert text on the displayed line
            this->insertNode(working_line, user_input);
            working_line++;

            break;

        case Insert:
            //TODO figure out how to/if you can decouple input/output
            //prompt with working line number and get input
            std::cout << range_start << "> ";
            getline(std::cin, user_input);

            this->insertNode(range_start, user_input);
            working_line = range_start + 1;

            break;

        case Revise:
            std::cout << range_start << "> ";
            getline(std::cin, user_input);

            this->editNode(range_start, user_input);
            //don't want to affect or use the "working line" for revisions

            break;

        case Delete:
            for (int i=range_start; i<=range_end; i++) this->deleteNode(range_start);
            //adjust working line if a line before it was deleted
            //TODO there has to be a better way of managing the working line
            if (working_line > range_end) {
                if (range_end - range_start != 0)
                    working_line = working_line - (range_end - range_start);
                working_line--;

                if (working_line < 1) working_line = 1;
            }

            break;

        case List:
            for (int i=range_start; i<=range_end; i++) this->printLine(i);
            working_line = range_end + 1;

            break;

        case Exit:
            //save file and leave
            std::stringstream editor_contents;
            for (int i=1; i<=size; i++) {
                editor_contents << this->getNodeData(i) << std::endl;
            }
            file_manager.writeStreamToFile(std::move(editor_contents));

            break;

    }//end switch operations
}
