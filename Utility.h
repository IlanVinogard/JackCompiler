#pragma once

#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <regex>
#include <unordered_set>
#include <fstream>
#include <sstream>

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
        getline(cin, path);
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

    static bool is_digits(const string& str) {
        for (char ch : str) {
            if (!isdigit(ch)) {
                return false;
            }
        }
        return true;
    }


};

class XmlTokenizer {
private:
    ofstream& output;
    unordered_set<string> keywords;
    unordered_set<char> symbols;

public:
    XmlTokenizer(ofstream& output) : output(output) {
        keywords = {
            "class", "constructor", "function", "method", "field", "static", "var",
            "int", "char", "boolean", "void", "true", "false", "null", "this",
            "let", "do", "if", "else", "while", "return"
        };
        symbols = {
            '{', '}', '(', ')', '.', ',', ';', '+', '-', '*', '/', '&', '|',
            '<', '>', '=', '~'
        };
    }

    string classifyToken(const string& token) {
        if (keywords.find(token) != keywords.end()) {
            return "<keyword> " + token + " </keyword>";
        }
        else if (isdigit(token[0])) {
            return "<integerConstant> " + token + " </integerConstant>";
        }
        else if (token.front() == '"' && token.back() == '"') {
            return "<stringConstant> " + token.substr(1, token.size() - 2) + " </stringConstant>";
        }
        else if (symbols.find(token[0]) != symbols.end()) {
            return "<symbol> " + token + " </symbol>";
        }
        else {
            return "<identifier> " + token + " </identifier>";
        }
    }

    void xmlTokenizer(const string& validLine) {
        bool inString = false;
        string currentToken;
        string tokenBuffer;

        for (char ch : validLine) {
            if (ch == '"') {
                if (inString) {
                    tokenBuffer += ch;
                    output << classifyToken(tokenBuffer) << endl;
                    tokenBuffer.clear();
                    inString = false;
                }
                else {
                    if (!currentToken.empty()) {
                        output << classifyToken(currentToken) << endl;
                        currentToken.clear();
                    }
                    tokenBuffer += ch;
                    inString = true;
                }
            }
            else if (inString) {
                tokenBuffer += ch;
            }
            else if (isspace(ch)) {
                if (!currentToken.empty()) {
                    output << classifyToken(currentToken) << endl;
                    currentToken.clear();
                }
            }
            else if (symbols.find(ch) != symbols.end()) {
                if (!currentToken.empty()) {
                    output << classifyToken(currentToken) << endl;
                    currentToken.clear();
                }
                output << "<symbol> " << ch << " </symbol>" << endl;
            }
            else {
                currentToken += ch;
            }
        }
        if (!currentToken.empty()) {
            output << classifyToken(currentToken) << endl;
        }
    }
};