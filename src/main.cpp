#include <iostream>
#include "../inc/LineEditor.h"

using namespace std;

int main(int argc, char *argv[]) {
    string user_input, file_name;

    //load/create the file given as command line argument
    if (argc == 3 && strcmp(argv[1], "edit") == 0) {
        file_name = argv[2];
    }

    LineEditor line_editor(file_name);

    //TODO this should probably be in the init function
    //show them the document they're working with at the start of the program
    for (int i=1; i<=line_editor.getSize(); i++)
        line_editor.printLine(i);

    do {
        //take user input and let the line editor do what it needs to with it
        cout << line_editor.getWorkingLine() << "> " ;
        getline(cin, user_input);
        line_editor.execute(user_input);

    } while (line_editor.isEditing());

    std::cout << "Contents saved to " << file_name << endl;
    cout << "Exiting application..." << endl;

    return 0;
}
