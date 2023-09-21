#include <iostream>
#include <sstream>
#include <vector>
#include "../inc/LineEditor.h"
#include "../inc/FileManager.h"

using namespace std;

enum Operations {
    Operation_Invalid,
    Insert,
    Revise,
    Delete,
    List,
    Exit
};

Operations resolveOperations(const string& input) {
    if (input == "I") return Insert;
    if (input == "R") return Revise;
    if (input == "D") return Delete;
    if (input == "L") return List;
    if (input == "E") return Exit;

    return Operation_Invalid;
}

//TODO this is awful
bool isValidOperation(const vector<string>& args) {
    //check that the first argument is a valid operation, then check if other args can be parsed to ints.
    //whether the number is a valid line number or not will be handled by the LineEditor class
    if (!args.empty() && resolveOperations(args.at(0)) != Operation_Invalid) {
        if (args.size() > 3) return false;
        if (args.size() > 1 && args.size() <= 3) {
            if (args.size() == 2) {
                char * p;
                strtol(args.at(1).c_str(), &p, 10);
                return * p == 0;
            } else if (args.size() == 3) {
                char * p1, * p2;
                strtol(args.at(1).c_str(), &p1, 10);
                strtol(args.at(2).c_str(), &p2, 10);
                return (*p1 == 0 && *p2 == 0);
            }
        }

        return true;
    }

    return false;
}

int main() {
    //LineEditor line_editor;
    Operations operation;
    string user_input, file_name;
    int working_line = 1;

    //take file name argument and load/create that file
    cout << "Enter file name (.txt extension only): ";
    getline(cin, file_name);

    //load file contents to stream to move to line editor
    FileManager file_manager(file_name);
    stringstream file_contents = file_manager.readStreamFromFile();
    LineEditor line_editor(std::move(file_contents));

    working_line = line_editor.getLastLine();
    cout << line_editor;

    do {
        //take user input and try to parse to a valid command
        cout << working_line << "> ";
        getline(cin, user_input);
        istringstream input_stream(user_input);

        //send file_contents_stream content to vector for easy parsing
        vector<string> args;
        string arg;
        while (input_stream >> arg) args.push_back(arg);
        if (args.empty()) continue;

        //assign operation and line number arguments
        //TODO this is messy
        int range_start = 0, range_end = 0;

        operation = resolveOperations(args.at(0));
        if (isValidOperation(args)) {
            if (args.size() >= 2) {
                range_start = stoi(args.at(1));
                if (operation == Insert) working_line = range_start;
            }
            if (args.size() == 3)
                range_end = stoi(args.at(2));
        } else {
            args.clear();
            operation = Operation_Invalid;
        }

        switch (operation) {
            case Exit:
                //get outta here
                break;

            case Operation_Invalid:
                //If they haven't entered a valid operation, assume they're trying to insert text on the displayed line
                line_editor.insertLine(working_line, user_input);
                working_line++;

                break;

            case Insert:
                //if they've given no arguments other than I, send them above the current working line.
                if (range_start == 0 && working_line != 1)
                    working_line--;

                //prompt with working line number and get input
                cout << working_line << "> ";
                getline(cin, user_input);
                line_editor.insertLine(working_line, user_input);
                working_line++;

                break;

            case Revise:
                //prompt with working line number and get input
                //special case for revise, don't want to affect the working line
                if (range_start != 0) cout << range_start << "> ";
                //if they've given no arguments other than I, send them above the current working line.
                if (range_start == 0 && working_line != 1) cout << working_line - 1 << "> ";

                getline(cin, user_input);
                if (range_start != 0)
                    line_editor.editLine(range_start, user_input);
                else
                    line_editor.editLine(working_line - 1, user_input);

                break;

            case Delete:
                //delete a range of lines, a specified line, or the most recent line
                if (range_start != 0 && range_end != 0) {
                    line_editor.deleteRange(range_start, range_end);
                } else if (range_start != 0) {
                    line_editor.deleteLine(range_start);
                } else {
                    line_editor.deleteLine(line_editor.getLastLine() - 1);
                }

                //adjust working line if a line before it was deleted
                if (working_line >= range_start) working_line--;

                break;

            case List:
                //print a range of lines, a single line, or the whole document
                if (range_start != 0 && range_end != 0) {
                    line_editor.printRange(range_start, range_end);
                    working_line = range_end + 1;
                } else if (range_start != 0) {
                    line_editor.printLine(range_start);
                    working_line = range_start + 1;
                } else {
                    cout << line_editor;
                    //reset the working line to end of doc
                    working_line = line_editor.getLastLine();
                }

                break;

        }//end switch operations
    } while (operation != Exit);

    //save to file
    stringstream editor_contents;
    for (int i=1; i<line_editor.getLastLine(); i++) {
        editor_contents << line_editor.getLine(i) << endl;
    }
    file_manager.writeStreamToFile(std::move(editor_contents));

    cout << "Exiting application..." << endl;

    return 0;
}
