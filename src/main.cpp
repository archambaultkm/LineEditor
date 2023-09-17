#include <iostream>
#include "../inc/LineEditor.h"

using namespace std;

int main() {

    LineEditor line_editor;
    string operation;
    string entered_text;
    int current_line = 1;

    while (operation != "E") {

        cout << "Enter an operation: ";
        cin >> operation;

        if (operation == "I") {
            cout << current_line << "> ";
            cin >> entered_text;
            line_editor.insertLine(current_line, entered_text);
        } else if (operation == "L") {
            cout << line_editor << endl;
        } else if (operation == "D") {
            line_editor.deleteLine(current_line);
        }

        current_line++;
    }

    return 0;
}