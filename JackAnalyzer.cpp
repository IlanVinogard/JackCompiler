#include <iostream>
#include "../Compiler/JackTokenizer.h"
#include "../Compiler/Utility.h"

int main() {
    try {
        while (true) {
            Ui::uiLogo();

            string directoryPath = File::askDirectoryPath();

            // Check for valid directory and clear console.
            bool isValidDir = File::isValidDirectory(directoryPath);
            Ui::clear();

            // If not valid directory, force user to re-enter.
            while (!isValidDir) {
                Ui::uiLogo();
                std::cout << "Invalid directory!" << std::endl;
                directoryPath = File::askDirectoryPath();
                isValidDir = File::isValidDirectory(directoryPath);
                Ui::clear();
            }

            vector<string> jackFiles = File::getJackFiles(directoryPath);

            if (jackFiles.empty()) {
                std::cerr << "No .jack files found in the directory!" << std::endl;
                continue;
            }

            JackTokenizer jackTokenizer;

            try {
                std::cout << "Files successfully opened" << std::endl;

                for (const auto& file : jackFiles) {
                    string xmlFileName = file + ".xml";

                    // Converting each .jack file to corresponding .xml file
                    std::cout << "Converting " << file << " to " << xmlFileName << std::endl;
                    jackTokenizer.convertFile(file, xmlFileName);

                    std::cout << "Finished converting " << file << " to " << xmlFileName << "!" << std::endl;
                }
            }
            catch (const std::runtime_error& e) {
                std::cerr << "Error during translating: " << e.what() << std::endl;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << '\n';
        return 1;
    }

    return 0;
}