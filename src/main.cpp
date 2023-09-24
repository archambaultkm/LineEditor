#include <iostream>
#include "../inc/LineEditor.h"

using namespace std;

int main(int argc, char *argv[]) {
    LineEditor line_editor;
    string user_input, file_name;

    if (argc == 3 && strcmp(argv[1], "edit") == 0) {
        file_name = argv[2];
    }

    //load/create the file given as command line argument
    line_editor.readFromFile(file_name);

    //print the loaded document at the start of the program
    for (int i=1; i<=line_editor.getSize(); i++)
        line_editor.printLine(i);

    do {
        //take user input and let the line editor figure out what to do with it
        cout << line_editor.getWorkingLine() << "> " ;
        getline(cin, user_input);
        line_editor.execute(user_input);

    } while (line_editor.isEditing());

    //when they're done, save the file.
    line_editor.writeToFile(file_name);
    std::cout << "Contents saved to " << file_name << endl;
    cout << "Exiting application..." << endl;

    return 0;
}
