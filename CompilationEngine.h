#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

class CompilationEngine {
public:
    CompilationEngine() = default;

    void convertFile(const string& inputPath, const string& outputPath) {
        input.open(inputPath);
        if (!input.is_open()) {
            cerr << "Error opening input file: " << inputPath << endl;
            throw runtime_error("Failed to open input file.");
        }

        output.open(outputPath);
        if (!output.is_open()) {
            cerr << "Error opening output file: " << outputPath << endl;
            throw runtime_error("Failed to open output file.");
        }

        input.close();
        output.close();
    }

private:
    ifstream input;
    ofstream output;

    string advance() {
        string line;
        if (getline(input, line)) {
            return line;
        }
        return "";
    };
};