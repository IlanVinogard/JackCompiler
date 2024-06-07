#include <iostream>
#include "../Compiler/Utility.h"
#include "../Compiler/JackTokenizer.h"

int main() {
    try {
        while (true) {
            Ui::uiLogo();

            string directoryPath = File::askDirectoryPath();

            // check for valid directory and clear console.
            bool isValidDir = File::isValidDirectory(directoryPath);
            Ui::clear();

            // if not valid directory force user to re-enter.
            while (!isValidDir) {
                Ui::uiLogo();
                cout << "Invalid directory!" << endl;
                directoryPath = File::askDirectoryPath();
                isValidDir = File::isValidDirectory(directoryPath);
                Ui::clear();
            }

            vector<string> jackFiles = File::getJackFiles(directoryPath);

            if (jackFiles.empty()) {
                cerr << "No .jack files found in the directory!" << endl;
                continue;
            }

            JackTokenizer jackTokenizer;

            try {
                cout << "Files successfully opened" << endl;

                for (const auto& file : jackFiles) {
                    string xmlFileName = file + ".xml";

                    // Converting each .jack file to corresponding .xml file
                    jackTokenizer.convertFile(file, xmlFileName);

                    cout << "Finished converting " << file << " to " << xmlFileName << "!" << endl;
                }
            }
            catch (const runtime_error& e) {
                cerr << "Error during translating: " << e.what() << endl;
            }
        }
    }
    catch (const exception& e) {
        cerr << "Unhandled exception: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
