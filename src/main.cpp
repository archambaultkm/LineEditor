#include <iostream>
#include <sstream>
#include <vector>
#include "../inc/LineEditor.h"
#include "../inc/FileManager.h"

using namespace std;

int main(int argc, char *argv[]) {
    string user_input, file_name;

    //load/create the file given as command line argument
    if (argc == 3 && strcmp(argv[1], "edit") == 0) {
        file_name = argv[2];
    }

    //TODO move into editor class
    //load file contents to stream to move to line editor
    FileManager file_manager(file_name);
    stringstream file_contents = file_manager.readStreamFromFile();
    LineEditor line_editor;

    line_editor.initContents(std::move(file_contents));

    //TODO this should probably be in the init function
    //show them the document they're working with at the start of the program
    for (int i=1; i<line_editor.getLastLine(); i++)
        line_editor.printLine(i);

    do {
        //take user input and let the line editor do what it needs to with it
        cout << line_editor.getWorkingLine() << "> " ;
        getline(cin, user_input);
        line_editor.execute(user_input);

    } while (line_editor.isEditing());

    //TODO move to editor class
    //save to file
    stringstream editor_contents;
    for (int i=1; i<line_editor.getLastLine(); i++) {
        editor_contents << line_editor.getLine(i) << endl;
    }

    file_manager.writeStreamToFile(std::move(editor_contents));
    cout << "Contents saved to " << file_name << endl;
    cout << "Exiting application..." << endl;

    return 0;
}
