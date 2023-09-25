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

LineEditor::Operation LineEditor::resolveOperations(const std::string& input) {
    if (input == "I") return Insert;
    if (input == "R") return Revise;
    if (input == "D") return Delete;
    if (input == "L") return List;
    if (input == "E") return Exit;

    return Not_An_Operation;
}

bool LineEditor::isValidOperation(const std::string& input) {
    std::regex valid_operation(R"(^(I|R|D|L|E)([[:space:]](-)?[0-9]+)?([[:space:]](-)?[0-9]+)?$)");

    return (regex_match(input, valid_operation));
}

void LineEditor::initOperations(const std::string& input) {
    range_start = 0, range_end = 0;
    std::vector<std::string> args;
    std::string arg;

    //use a stream and vector for easy parsing
    std::istringstream input_stream(input);
    while (input_stream >> arg) args.push_back(arg);

    operation = resolveOperations(args.at(0));
    //this moves the cursor up a line if the user gives no range argument for insert, edit, or delete
    if (working_line != 1) range_start = working_line - 1;

    //List has a special case for no number arguments
    if (args.size() == 1 && operation == List) {
        range_start = 1;
        range_end = size;
        if (range_end == 0) range_end = 1;

        return;
    }

    //reassign range numbers if values were provided
    if (args.size() >= 2) {
        try {
            range_start = stoi(args.at(1));
        } catch(std::out_of_range& e) { return; }

        //if 0 is given explicitly as an argument, stop setting values so that the operation fails (nothing happens)
        if (range_start == 0) return;
    }

    if (args.size() == 3) {
        try {
        range_end = stoi(args.at(2));
        } catch(std::out_of_range& e) { return; }

    } else {
        //for looping purposes, range end=start unless user provided a value
        range_end = range_start;
    }

    //make sure the range is positive
    if (range_start > range_end && range_end != 0) {
        int temp = range_end;
        range_end = range_start;
        range_start = temp;
    }
}

void LineEditor::execute(const std::string& input) {
    if (!isValidOperation(input)) {
        user_input = input;
        operation = Not_An_Operation;
    } else initOperations(input);

    switch (operation) {
        case Not_An_Operation:
            //If they haven't entered a valid operation, assume they're trying to insert text on the displayed line
            this->insertNode(working_line, user_input);
            working_line++;

            break;

        case Exit:

            break;

        case Insert:
            if (range_start <= 0 ||  range_end > size) return;
            //"Insert" just moves the line
            working_line = range_start;

            break;

        case Revise:
            if (range_start <= 0 ||  range_end > size) return;
            //I think decoupling io for this is more work than it's worth
            std::cout << range_start << "> ";
            getline(std::cin, user_input);

            this->editNode(range_start, user_input);

            break;

        case Delete:
            if (range_start <= 0 ||  range_end > size) return;
            for (int i=range_start; i<=range_end; i++) this->deleteNode(range_start);

            //adjust working line if a line before it was deleted
            if (working_line >= range_end) {
                if (range_end - range_start != 0)
                    working_line = working_line - (range_end - range_start);

                working_line--;

                //don't allow the working line to go below the first line of the document
                if (working_line < 1) working_line = 1;
            }

            break;

        case List:
            if (range_start <= 0 ||  range_end > size) return;
            for (int i=range_start; i<=range_end; i++) {
                //don't allow printing of lines outside document bounds
                if (i <= 0 || i > size) break;
                this->printLine(i);
            }

            if (range_end > 1 && range_end < size) working_line = range_end + 1;

            break;
    }//end switch operations
}

void LineEditor::readFromFile(const std::string& file_name) {
    std::string file_line;

    try {
        ifs.open(file_name, std::fstream::in);

        if (ifs.fail()) {
            std::cout << "Could not find " << file_name << ", creating new document." << std::endl;
        }

        //take each line from file (even if empty) and insert into line editor
        int line_num = 1;
        while (getline(ifs, file_line)) {
            this->insertNode(line_num, file_line);
            line_num ++;
        }

    }  catch (std::ifstream::failure &e) {
        std::cout << "Exception opening/closing/reading file" << std::endl;
    }

    working_line = size + 1;
    ifs.close();
}

void LineEditor::writeToFile(const std::string& file_name) {
    std::string file_line;

    try {
        //trunc: completely overwrite the old file
        ofs.open(file_name, std::fstream::trunc );

        if (ofs.fail()) {
            std::cout << "Could not open file.";
        }

        for (int i=1; i<= size; i++) {
            file_line = this->getNodeData(i);
            ofs << file_line << std::endl;
        }

    }  catch (std::ofstream::failure &e) {
        std::cout << "Exception opening/closing/reading file" << std::endl;
    }

    ofs.close();
}
