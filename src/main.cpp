#include <iostream>
#include <sstream>
#include <vector>
#include "../inc/LineEditor.h"

using namespace std;

bool isValidOperation(const vector<string>& args) {

    bool valid_operation;

    if (!args.empty()) valid_operation = args.at(0) == "L" ||
                      args.at(0) == "D" ||
                      args.at(0) == "I" ||
                      args.at(0) == "R" ||
                      args.at(0) == "E";

    if (!args.empty() && valid_operation) {
        if (args.size() > 1 && args.size() <= 3) {
            if (args.size() == 2) {
                char* p;
                strtol(args.at(1).c_str(), &p, 10);
                return *p == 0;
            } else if (args.size() == 3) {
                char* p;
                strtol(args.at(1).c_str(), &p, 10);
                return *p == 0;
            }
        }

        return true;
    }

    return false;
}

int main() {

    LineEditor line_editor;
    string user_input, operation;
    int last_line, working_line = 1;

    //TODO start of program, take file name argument and load that file

    while (operation != "E") {

        //update displayed line based on data from line editor for readability
        last_line = line_editor.getSize() + 1;
        int range_start = 0, range_end = 0;

        //take user input and try to parse to a valid command
        cout << working_line << "> ";
        getline(cin, user_input);
        istringstream ss(user_input);

        //send ss content to vector for easy parsing
        vector<string> args;
        string arg;
        while (ss >> arg) args.push_back(arg);


        //If they haven't entered a valid operation, assume they're trying to insert text on the displayed line
        if (!isValidOperation(args)) {
            line_editor.insertLine(working_line, user_input);
            working_line++;

        } else {
            //assign operation and line number arguments
            operation = args.at(0);
            if (args.size() >= 2) {
                range_start = stoi(args.at(1));
                working_line = range_start;
            }
            if (args.size() == 3)
                range_end = stoi(args.at(2));

            if (operation == "I") {
                //if they've given no arguments other than I, send them above the current working line.
                if (range_start == 0)
                    working_line--;

                //prompt with working line number and get input
                cout << working_line << "> ";
                getline(cin, user_input);
                line_editor.insertLine(working_line, user_input);
                working_line++;

            //print a range of lines, a single line, or the whole document
            } else if (operation == "L") {
                if (range_start != 0 && range_end != 0)
                    line_editor.printRange(range_start, range_end);
                else if (range_start != 0)
                    line_editor.printLine(range_start);
                else
                    cout << line_editor;

                //reset the working line to end of doc
                working_line = last_line;

            //delete a range of lines, a specified line, or the most recent line
            } else if (operation == "D") {

                if (range_start != 0 && range_end != 0)
                    line_editor.deleteRange(range_start, range_end);
                else if (range_start != 0)
                    line_editor.deleteLine(range_start);
                else
                    line_editor.deleteLine(last_line - 1);

                //reset the working line to end of doc
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

                //reset the working line to end of doc
                working_line = last_line;
            }
        }//end operations
    }//end while loop

    //TODO here save to file
    cout << "Exiting application..." << endl;

    return 0;
}
