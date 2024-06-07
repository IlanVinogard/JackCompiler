#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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

        CodeWriter codeWriter(output);

        bool advComment = false;
        string line;
        while (getline(input, line)) {
            line = Parser::removeComments(line);
            if (Parser::isNotCommentLine(line, advComment)) {
                try {
                    string validLine = Parser::cleanAndValidateLine(line);
                    // continue code.
                }
                catch (const runtime_error& e) {
                    cerr << "Error: " << e.what() << endl;
                }
            }
        }

        input.close();
        output.close();
    }
};
