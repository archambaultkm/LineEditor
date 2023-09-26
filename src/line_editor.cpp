//
// Created by Kaitlyn Archambault on 2023-09-16.
//

#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include "../inc/line_editor.h"

LineEditor::LineEditor() {
    m_operation = Not_An_Operation;
    m_iter_start = 0;
    m_iter_end = 0;
    m_working_line = 1;
}

LineEditor::~LineEditor() = default;

int LineEditor::getWorkingLine() const {
    return this->m_working_line;
}

bool LineEditor::isEditing() {
    return m_operation != Exit;
}

/**
 * << operator prints entire contents of a line editor
 *
 */
std::ostream &operator<<(std::ostream &output, LineEditor& line_editor) {
    int line_num = 1;

    //output each line of the document
    while (line_num <= line_editor.m_size) {
        std::cout << line_num << "> " << line_editor.getNodeData(line_num) << std::endl;
        line_num++;
    }

    return output;
}

/**
 * Match input string to an available m_operation
 *
 * @param input provided letter
 */
LineEditor::Operation LineEditor::resolveOperations(const std::string& input) {
    if (input == "I") return Insert;
    if (input == "R") return Revise;
    if (input == "D") return Delete;
    if (input == "L") return List;
    if (input == "E") return Exit;

    return Not_An_Operation;
}

/**
 * Check if user input can be parsed to a valid m_operation for the line editor to use
 *
 * @param input entire line input by the user
 * @note the expected formats for an m_operation are "A", "A int1", or "A int1 int2"
 * @returns bool- input line matches regex expression
 */
bool LineEditor::isValidOperation(const std::string& input) {
    std::regex valid_operation(R"(^(I|R|D|L|E)([[:space:]](-)?[0-9]+)?([[:space:]](-)?[0-9]+)?$)");

    return (regex_match(input, valid_operation));
}

/**
 * If the input was successfully validated as an m_operation, initialize the
 * Line Editor's m_operation, m_iter_start, and m_iter_end properties
 *
 * @param input entire line input by the user
 * @note this function also ensures iteration range is positive
 */
void LineEditor::initOperations(const std::string& input) {
    m_iter_start = 0, m_iter_end = 0;
    std::vector<std::string> args;
    std::string arg;

    // use a stream and vector for easy parsing
    std::istringstream input_stream(input);
    while (input_stream >> arg) args.push_back(arg);

    m_operation = resolveOperations(args.at(0));
    // this moves the cursor up a line if the user gives no range argument for insert, edit, or delete
    if (m_working_line != 1) m_iter_start = m_working_line - 1;

    // List has a special case for no number arguments
    if (args.size() == 1 && m_operation == List) {
        m_iter_start = 1;
        m_iter_end = m_size;
        if (m_iter_end == 0) m_iter_end = 1;

        return;
    }

    // reassign iteration range if values were provided
    if (args.size() >= 2) {
        try {
            m_iter_start = stoi(args.at(1));
        } catch(std::out_of_range& e) { return; }

        // user is re-prompted for input if 0 is given explicitly as an argument
        if (m_iter_start == 0) return;
    }

    if (args.size() == 3) {
        try {
            m_iter_end = stoi(args.at(2));
        } catch(std::out_of_range& e) { return; }

    } else {
        // for looping purposes, iter end=start unless user provided a value
        m_iter_end = m_iter_start;
    }

    // make sure the range is positive
    if (m_iter_start > m_iter_end && m_iter_end != 0) {
        int temp = m_iter_end;
        m_iter_end = m_iter_start;
        m_iter_start = temp;
    }
}

/**
 * Invokes private functions to validate the given input and
 * determine what m_operation to execute. Any cases that change the
 * m_size property of the line editor (Insert or Delete) will also
 * manage updating the current working line.
 *
 * @param input entire line input by the user
 * @note this is meant to take responsibility for control flow and
 * validation away from the main program so that the class is reusable
 * anywhere
 */
void LineEditor::execute(const std::string& input) {
    if (!isValidOperation(input)) {
        m_user_input = input;
        m_operation = Not_An_Operation;
    } else initOperations(input);

    switch (m_operation) {
        case Not_An_Operation:
            // If they haven't entered a valid m_operation, assume they're trying to insert text on the displayed line
            this->insertNode(m_working_line, m_user_input);
            m_working_line++;

            break;

        case Exit:

            break;

        case Insert:
            if (m_iter_start <= 0 || m_iter_end > m_size) return;
            // "Insert" just moves the line
            m_working_line = m_iter_start;

            break;

        case Revise:
            if (m_iter_start <= 0 || m_iter_end > m_size) return;

            std::cout << m_iter_start << "> ";
            getline(std::cin, m_user_input);

            this->editNode(m_iter_start, m_user_input);

            break;

        case Delete:
            if (m_iter_start <= 0 || m_iter_end > m_size) return;
            for (int i=m_iter_start; i <= m_iter_end; i++) this->deleteNode(m_iter_start);

            // adjust working line if a line before it was deleted
            if (m_working_line >= m_iter_end) {
                if (m_iter_end - m_iter_start != 0)
                    m_working_line = m_working_line - (m_iter_end - m_iter_start);

                m_working_line--;

                //don't allow the working line to go below the first line of the document
                if (m_working_line < 1) m_working_line = 1;
            }

            break;

        case List:
            if (m_iter_start <= 0 || m_iter_end > m_size) return;
            for (int i=m_iter_start; i <= m_iter_end; i++) {
                // don't allow printing of lines outside document bounds
                if (i <= 0 || i > m_size) break;
                std::cout << i << "> " << this->getNodeData(i) << std::endl;
            }

            if (m_iter_end > 1 && m_iter_end <= m_size) m_working_line = m_iter_end + 1;

            break;
    } //end switch operations
}

/**
 * Use fstream to read in file and insert into line editor line
 * by line.
 *
 * @param file_name already-validated input file path
 * @note also adjusts working line for the editor to end
 * of file.
 */
void LineEditor::readFromFile(const std::string& file_name) {
    std::string file_line;

    try {
        m_ifs.open(file_name, std::fstream::in);

        if (m_ifs.fail()) {
            std::cout << "Could not find " << file_name << ", creating new document." << std::endl;
        }

        // take each line from file (even if empty) and insert into line editor
        int line_num = 1;
        while (getline(m_ifs, file_line)) {
            this->insertNode(line_num, file_line);
            line_num ++;
        }

    }  catch (std::ifstream::failure &e) {
        std::cout << "Exception opening/closing/reading file" << std::endl;
    }

    m_working_line = m_size + 1;
    m_ifs.close();
}

/**
 * Use fstream to write the contents of the Line Editor to
 * file line by line
 *
 * @param file_name already-validated input file path
 */
void LineEditor::writeToFile(const std::string& file_name) {
    std::string file_line;

    try {
        // trunc: completely overwrite the old file
        m_ofs.open(file_name, std::fstream::trunc );

        if (m_ofs.fail()) {
            std::cout << "Could not open file.";
        }

        for (int i=1; i <= m_size; i++) {
            file_line = this->getNodeData(i);
            m_ofs << file_line << std::endl;
        }

    }  catch (std::ofstream::failure &e) {
        std::cout << "Exception opening/closing/reading file" << std::endl;
    }

    m_ofs.close();
}
