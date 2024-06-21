#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include "../Compiler/Utility.h"
using namespace std;

class JackTokenizer {
public:
    JackTokenizer() = default;

    void convertFile(const string& inputPath, const string& outputPath) {
        ifstream input(inputPath);
        if (!input.is_open()) {
            cerr << "Error opening input file: " << inputPath << endl;
            throw runtime_error("Failed to open input file.");
        }

        ofstream output(outputPath);
        if (!output.is_open()) {
            cerr << "Error opening output file: " << outputPath << endl;
            throw runtime_error("Failed to open output file.");
        }
        cout << "Success open file!" << endl;

        XmlTokenizer xmltokenizer(output);

        cout << "created Token xml" << endl;

        bool advComment = false;
        string line;
        output << "<tokens>" << endl;
        
        while (getline(input, line)) {
            line = Parser::removeComments(line);
            if (Parser::isNotCommentLine(line, advComment)) {
                try {
                    string validLine = Parser::cleanAndValidateLine(line);
                    if (!validLine.empty()) {
                        xmltokenizer.xmlTokenizer(validLine);
                    }
                }
                catch (const runtime_error& e) {
                    cerr << "Error: " << e.what() << endl;
                }
            }
        }

        output << "</tokens>" << endl;

        input.close();
        output.close();
    }
};
