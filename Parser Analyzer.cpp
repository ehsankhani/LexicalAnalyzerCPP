#include <iostream>
#include <string>
#include <regex>
//#include <sstream>

using namespace std;

// Function to remove comments from a line of code
string removeComments(const string& line) {
    regex commentRegex(R"(\/\/.*$|\/\*.*\*\/)");
    return regex_replace(line, commentRegex, "");
}

// Function to trim whitespace from the beginning and end of a string
string trim(const string& str) {
    size_t first = str.find_first_not_of(' ');
    size_t last = str.find_last_not_of(' ');
    if (first == string::npos || last == string::npos) {
        return "";
    }
    return str.substr(first, (last - first + 1));
}

// Function to analyze C++ code and categorize different components
void analyzeCode(const string& code) {
    istringstream stream(code);
    string line;

    regex classRegex(R"(\bclass\s+(\w+))");
    regex functionRegex(R"(\b(\w[\w\s\*&]*)\s+(\w+)\s*\(([^)]*)\)\s*(\{|;))");
    regex initializerListRegex(R"(\s*:\s*(.*)\{)");
    regex variableRegex(R"((\w[\w\s\*&]*)\s+(\w+)\s*=\s*(.*);)");
    regex simpleVarRegex(R"((\w[\w\s\*&]*)\s+(\w+);)");
    regex memberVarRegex(R"(\b(int|char|float|double|bool|string)\s+(\w+);)");
    regex conditionalRegex(R"(\b(if|else\s+if|else)\b)");
    regex loopRegex(R"(\b(for|while)\b)");
    regex functionCallRegex(R"((\w+)\s*\(([^)]*)\);)");
    regex mathOperationRegex(R"((\d+)\s*([\+\-\*\/])\s*(\d+))");

    bool inClassDefinition = false;
    string className;

    while (getline(stream, line)) {
        line = removeComments(line);
        line = trim(line);
        if (line.empty()) continue;

        smatch match;

        if (regex_search(line, match, classRegex)) {
            className = match[1];
            cout << "Class name: " << className << endl;
            inClassDefinition = true;
        } else if (inClassDefinition && regex_search(line, match, functionRegex)) {
            cout << "Function: " << match[2] << endl;
            cout << "  Return type: " << match[1] << endl;
            cout << "  Arguments: " << match[3] << endl;
            if (getline(stream, line) && regex_search(line, match, initializerListRegex)) {
                cout << "  Initializer list: " << match[1] << endl;
            }
        } else if (regex_search(line, match, memberVarRegex) || regex_search(line, match, variableRegex)) {
            if (regex_search(line, match, memberVarRegex)) {
                cout << "Variable (member): " << match[2] << endl;
                cout << "  Type: " << match[1] << endl;
            } else {
                cout << "Variable: " << match[2] << endl;
                cout << "  Type: " << match[1] << endl;
                cout << "  Initial value: " << match[3] << endl;
            }
        } else if (regex_search(line, match, simpleVarRegex)) {
            cout << "Variable: " << match[2] << endl;
            cout << "  Type: " << match[1] << endl;
        } else if (regex_search(line, match, conditionalRegex)) {
            cout << "Conditional: " << match[1] << endl;
        } else if (regex_search(line, match, loopRegex)) {
            cout << "Loop: " << match[1] << endl;
        } else if (regex_search(line, match, functionCallRegex)) {
            cout << "Function call: " << match[1] << endl;
            cout << "  Parameters: " << match[2] << endl;
        } else if (regex_search(line, match, mathOperationRegex)) {
            int lhs = stoi(match[1]);
            int rhs = stoi(match[3]);
            int result = 0;
            if (match[2] == "+") {
                result = lhs + rhs;
            } else if (match[2] == "-") {
                result = lhs - rhs;
            } else if (match[2] == "*") {
                result = lhs * rhs;
            } else if (match[2] == "/") {
                result = lhs / rhs;
            }
            cout << "Math operation result: " << result << endl;
        }
    }
}

int main() {
    string code;
    string line;
    cout << "Enter your C++ code (type 'END' on a new line to finish):" << endl;
    while (getline(cin, line)) {
        if (line == "END") break;
        code += line + "\n";
    }

    analyzeCode(code);

    return 0;
}
