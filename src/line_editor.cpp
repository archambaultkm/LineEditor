/**
 * @author Kaitlyn Archambault
 * @date Sept. 26, 2023
 */

#include <iostream>
#include <sstream>
#include <vector>
#include <regex>
#include "../inc/line_editor.h"

LineEditor::LineEditor() :
    m_operation(Not_An_Operation),
    m_iter_start(0),
    m_iter_end(0),
    m_working_line(1)
    {}

LineEditor::~LineEditor() = default;

int LineEditor::getWorkingLine() const {
    return this->m_working_line;
}

bool LineEditor::isEditing() {
    return m_operation != Exit;
}

/**
 * Invokes private functions to validate the given input and
 * determine what operation to execute. Any cases that change the
 * m_size property of the line editor (Insert or Delete) will also
 * manage updating the current working line.
 *
 * @param input entire line input by the user
 * @note this is meant to take responsibility for control flow and
 * validation away from the main program so that the class is reusable
 * anywhere
 */
void LineEditor::execute(std::string& input) {
    if (!isValidOperation(input)) {
        m_operation = Not_An_Operation;
    } else initOperations(input);

    // any case other than "not an operation" needs to have a possible iteration range
    if (m_operation != Not_An_Operation && (m_iter_start <= 0 || m_iter_end > m_size)) return;

    switch (m_operation) {
        case Not_An_Operation:
            // If they haven't entered a valid operation, assume they're trying to insert text on the displayed line
            this->insertNode(m_working_line, input);
            m_working_line++;

            break;

        case Insert:
            // "Insert" just moves the cursor
            m_working_line = m_iter_start;

            break;

        case Revise:
            std::cout << m_iter_start << "> ";
            getline(std::cin, input);

            this->editNode(m_iter_start, input);

            break;

        case Delete:
            for (int i=m_iter_start; i <= m_iter_end; i++)
                this->deleteNode(m_iter_start);

            //don't allow the working line to exceed the end of the document
            if (m_working_line > m_size) m_working_line = m_size + 1;

            break;

        case List:
            //if only one line is requested, print just that line
            if (m_iter_end == m_iter_start) {
                std::cout << m_iter_start << "> " << this->getNodeData(m_iter_start) << std::endl;

            } else {
                //rather than iterating through the whole list for each line, use a function that returns a vector of node data
                std::vector<std::string> lines_to_print = this->getNodeData(m_iter_start, m_iter_end);

                //the line prompt is not related to the vector index of the data being printed, so assign it separately.
                int line_prompt = m_iter_start;

                for (const auto & i : lines_to_print) {
                    std::cout << line_prompt << "> " << i << std::endl;
                    line_prompt++;
                }
            }

            //set the working line to directly after the end of whatever got printed
            if (m_iter_end >= 1 && m_iter_end <= m_size) m_working_line = m_iter_end + 1;

            break;

        case Exit:

            break;
    }
}

/**
 * Match input string to an available operation
 *
 * @param input Provided letter split from input arguments
 *
 * @returns Operation to be performed by the LineEditor
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
 * Check if user input can be parsed to a valid operation for the line editor to use
 *
 * @param input entire line input by the user
 * @note the expected formats for an operation are "A", "A int1", or "A int1 int2"
 * @returns bool- input line matches regex expression
 */
bool LineEditor::isValidOperation(const std::string& input) {
    std::regex valid_operation(R"(^(I|R|D|L|E)([[:space:]](-)?[0-9]+)?([[:space:]](-)?[0-9]+)?$)");

    return (regex_match(input, valid_operation));
}

/**
 * If the input was successfully validated as an operation, initialize the
 * Line Editor's m_operation, m_iter_start, and m_iter_end properties
 *
 * @param input entire line input by the user
 * @note this function also ensures iteration range is positive
 */
void LineEditor::initOperations(const std::string& input) {
    m_iter_start = 0, m_iter_end = 0;

    // Split the input string into arguments
    std::istringstream input_stream(input);
    std::vector<std::string> args(std::istream_iterator<std::string>{input_stream},
                                  std::istream_iterator<std::string>());

    if (args.empty()) return; // No valid arguments

    // Determine the operation
    m_operation = resolveOperations(args.at(0));

    // List operation has default values for if no range is provided
    if (m_operation == List) {
        m_iter_start = 1;
        m_iter_end = (m_size > 0) ? m_size : 1;
    }

    // parse m_iter_start and m_iter_end if a value was provided
    if (args.size() >= 2) {
        try {
            m_iter_end = m_iter_start = stoi(args[1]);

            if (m_iter_start == 0) return; // User is re-prompted for input
        } catch (std::out_of_range& e) {
            return;
        }
    }

    // parse m_iter_end if a value was provided
    if (args.size() == 3) {
        try {
            m_iter_end = stoi(args[2]);
        } catch (std::out_of_range& e) {
            return;
        }
    }

    // Ensure the range is positive
    if (m_iter_start > m_iter_end && m_iter_end != 0) {
        std::swap(m_iter_start, m_iter_end);
    }
}

/**
 * \<\< operator returns entire contents of a line editor
 *
 */
std::ostream &operator<<(std::ostream &output, LineEditor& line_editor) {
    int line_num = 1;

    //output each line of the document
    while (line_num <= line_editor.m_size) {
        output << line_num << "> " << line_editor.getNodeData(line_num) << std::endl;
        line_num++;
    }

    return output;
}

/**
 * \<\< operator returns contents of line editor without line number prompts
 *
 * @note Used when the output will be saved to a file
 */
std::stringstream &operator<<(std::stringstream &output, LineEditor& line_editor) {
    int line_num = 1;

    //output each line of the document
    while (line_num <= line_editor.m_size) {
        output << line_editor.getNodeData(line_num) << std::endl;
        line_num++;
    }

    return output;
}

/**
 * \>\> operator reads contents of provided stream into line editor
 *
 */
std::istream &operator>>(std::istream& input, LineEditor& line_editor) {
    std::string stream_line;
    int line_num = 1;

    while (getline(input, stream_line)) {
        line_editor.insertNode(line_num, stream_line);
        line_num ++;
    }

    line_editor.m_size = line_num - 1;
    line_editor.m_working_line = line_editor.m_size + 1;
    return input;
}
