#include <iostream>
#include <vector>
#include <filesystem>
#include "../Compiler/Utility.h"
#include "../Compiler/JackTokenizer.h"

int main() {
    try {
        while (true) {
            Ui::uiLogo();

            std::string directoryPath = File::askDirectoryPath();

            // Check for valid directory and clear console.
            bool isValidDir = File::isValidDirectory(directoryPath);
            Ui::clear();

            // If not a valid directory, force user to re-enter.
            while (!isValidDir) {
                Ui::uiLogo();
                std::cout << "Invalid directory!" << std::endl;
                directoryPath = File::askDirectoryPath();
                isValidDir = File::isValidDirectory(directoryPath);
                Ui::clear();
            }

            std::vector<std::string> jackFiles = File::getJackFiles(directoryPath);

            if (jackFiles.empty()) {
                std::cerr << "No .jack files found in the directory!" << std::endl;
                continue;
            }

            JackTokenizer jackTokenizer;

            try {
                std::cout << "Files successfully opened" << std::endl;

                for (const auto& file : jackFiles) {
                    std::filesystem::path filePath(file);
                    filePath.replace_extension(".xml");
                    std::string xmlFileName = filePath.string();

                    // Converting each .jack file to corresponding .xml file
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