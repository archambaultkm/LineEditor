#include <iostream>
#include <sstream>
#include <vector>
#include "../inc/LineEditor.h"

using namespace std;

bool isValidOperation(const vector<string>& args) {
    if (!args.empty()) return
        (args.size() <= 3 &&
        args.at(0) == "L" ||
        args.at(0) == "D" ||
        args.at(0) == "I" ||
        args.at(0) == "R" ||
        args.at(0) == "E" );

    else return false;
}

int main() {

    LineEditor line_editor;
    string user_input, operation;
    int last_line, working_line = 1;

    while (operation != "E") {

        //update displayed line based on data from line editor for readability
        last_line = line_editor.getSize() + 1;
        int range_start = 0, range_end = 0;

        //take user input and try to parse to a valid command
        cout << working_line << "> ";
        getline(cin, user_input);
        istringstream ss(user_input);

        //send ss content to vector for easy parsing
        vector<string> words;
        string word;
        while (ss >> word) words.push_back(word);\

        //If they haven't entered a valid operation, assume they're trying to insert text on the displayed line
        if (!isValidOperation(words)) {
            line_editor.insertLine(working_line, user_input);
            working_line++;

        } else {
            operation = words.at(0);

            //attempt to parse line number arguments
            if (words.size() >= 2) {
                if (stoi(words.at(1))) range_start = stoi(words.at(1));
                working_line = range_start;
            }
            if (words.size() == 3)
                if (stoi(words.at(2))) range_end = stoi(words.at(2));

            if (operation == "I") {
                //prompt with working line number and get input
                if (range_start == 0)
                    working_line--;

                cout << working_line << "> ";
                getline(cin, user_input);
                line_editor.insertLine(working_line, user_input);
                working_line++;

            //print a range of lines, a single line, or the whole document
            } else if (operation == "L") {
                working_line = last_line;

                if (range_start != 0 && range_end != 0)
                    line_editor.printRange(range_start, range_end);
                else if (range_start != 0)
                    line_editor.printLine(range_start);
                else
                    cout << line_editor;

            //delete a range of lines, a specified line, or the most recent line
            } else if (operation == "D") {

                if (range_start != 0 && range_end != 0)
                    line_editor.deleteRange(range_start, range_end);
                else if (range_start != 0)
                    line_editor.deleteLine(range_start);
                else
                    line_editor.deleteLine(last_line - 1);

                working_line = last_line;

            //for funsies, "r" to revise an existing line
            } else if (operation == "R") {
                //prompt with working line number and get input
                cout << working_line << "> ";
                getline(cin, user_input);
                if (range_start != 0)
                    line_editor.editLine(range_start, user_input);
                else
                    line_editor.editLine(working_line, user_input);
            }
        }
    }

    return 0;
}
