#pragma once

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <regex>

using namespace std;
namespace fs = filesystem;

class Ui {
public:
    static void uiLogo() {
        cout << "\n\n";
        cout << "       _                  _       _____                               _   _               \n";
        cout << "      | |                | |     / ____|                             (_) | |              \n";
        cout << "      | |   __ _    ___  | | __ | |        ___    _ __ ___    _ __    _  | |   ___   _ __ \n";
        cout << "  _   | |  / _` |  / __| | |/ / | |       / _ \\  | '_ ` _ \\  | '_ \\  | | | |  / _ \\ | '__|\n";
        cout << " | |__| | | (_| | | (__  |   <  | |____  | (_) | | | | | | | | |_) | | | | | |  __/ | |   \n";
        cout << "  \\____/   \\__,_|  \\___| |_|\\_\\  \\_____|  \\___/  |_| |_| |_| | .__/  |_| |_|  \\___| |_|   \n";
        cout << "                                                             | |                          \n";
        cout << "                                                             |_|                          \n";
    }

    static void clear() {
        cout << "\x1B[2J\x1B[H";
    }
};

class File {
public:
    static string askDirectoryPath() {
        cout << "Please insert directory path containing .jack files\n";
        cout << "Directory path -> ";
        string path;
        cin >> path;
        return path;
    }

    static bool isValidDirectory(const string& path) {
        return fs::is_directory(path);
    }

    static vector<string> getJackFiles(const string& directoryPath) {
        vector<string> jackFiles;
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (entry.path().extension() == ".jack") {
                jackFiles.push_back(entry.path().string());
            }
        }
        return jackFiles;
    }
};

class Parser {
public:
    static string cleanAndValidateLine(const string& line) {
        regex pattern("\\s+");
        string validLine = regex_replace(line, pattern, " ");

        // Remove leading and trailing spaces
        size_t firstNonSpace = validLine.find_first_not_of(" ");
        size_t lastNonSpace = validLine.find_last_not_of(" ");

        if (firstNonSpace != string::npos && lastNonSpace != string::npos) {
            validLine = validLine.substr(firstNonSpace, lastNonSpace - firstNonSpace + 1);
        }
        else {
            validLine = "";
        }

        return validLine;
    }

    static bool isNotCommentLine(const string& line, bool& inMultilineComment) {
        string trimmedLine = cleanAndValidateLine(line);

        if (inMultilineComment) {
            size_t endComment = trimmedLine.find("*/");
            if (endComment != string::npos) {
                inMultilineComment = false;
                trimmedLine = trimmedLine.substr(endComment + 2);
            }
            else {
                return false;
            }
        }

        size_t startComment = trimmedLine.find("/*");
        if (startComment != string::npos) {
            size_t endComment = trimmedLine.find("*/", startComment + 2);
            if (endComment != string::npos) {
                trimmedLine = trimmedLine.substr(0, startComment) + trimmedLine.substr(endComment + 2);
            }
            else {
                inMultilineComment = true;
                trimmedLine = trimmedLine.substr(0, startComment);
            }
        }

        trimmedLine = cleanAndValidateLine(trimmedLine);

        return !trimmedLine.empty();
    }

    static string removeComments(const string& line) {
        size_t commentStart = line.find("//");
        if (commentStart != string::npos) {
            return line.substr(0, commentStart);
        }
        return line;
    }
};

class CodeWriter {
private:
    ofstream& output;

public:
    CodeWriter(ofstream& output) : output(output) {};

    void xmlWriter(const string& validLine) {
        // continue code.
    }

    void tokenType(const string& token) {
        // continue code.
    }
};