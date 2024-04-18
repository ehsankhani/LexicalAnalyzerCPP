#include <iostream>
#include <string>
#include <cctype>

using namespace std;

enum TokenType {
    KEYWORD,
    IDENTIFIER,
    INTEGER,
    FLOAT_SINGLE,
    FLOAT_DOUBLE,
    WHITESPACE,
    DEFAULT_SIGN,
    OPERATOR,
    LINE_COMMENT,
    SECTION_COMMENT
};

struct Token {
    TokenType type;
    string value;
    int line;
    int column;
};

bool isKeyword(const string& token) {
    static const string keywords[] = {"true", "for", "do", "begin", "try", "if", "double", "bool", "while", "int", "else", "break", "void", "return", "end", "case", "false", "char", "switch", "float", "class"};
    for (const auto& keyword : keywords) {
        if (token == keyword) {
            return true;
        }
    }
    return false;
}

bool isOperator(char c) {
    static const string operators = "+-*/%=<>,;()[]{}";
    return operators.find(c) != string::npos;
}

Token getNextToken(const string& line, size_t& pos, int lineNumber) {
    Token token;
    token.line = lineNumber;
    token.column = pos + 1;

    // Skip white spaces
    while (pos < line.size() && isspace(line[pos])) {
        pos++;
        token.type = WHITESPACE;
    }

    if (pos >= line.size()) {
        token.type = WHITESPACE;
        return token;
    }

    // Check for comments
    if (line[pos] == '/') {
        if (pos + 1 < line.size() && line[pos + 1] == '/') {
            token.type = LINE_COMMENT;
            return token;
        } else if (pos + 1 < line.size() && line[pos + 1] == '*') {
            token.type = SECTION_COMMENT;
            return token;
        }
    }

    // Check for identifiers or keywords
    if (isalpha(line[pos]) || line[pos] == '_') {
        token.type = IDENTIFIER;
        while (pos < line.size() && (isalnum(line[pos]) || line[pos] == '_')) {
            token.value += line[pos];
            pos++;
        }
        if (isKeyword(token.value)) {
            token.type = KEYWORD;
        }
        return token;
    }

    // Check for integers or floats
    if (isdigit(line[pos])) {
        token.type = INTEGER;
        while (pos < line.size() && (isdigit(line[pos]) || line[pos] == '.' || line[pos] == 'f')) {
            token.value += line[pos];
            pos++;
        }
        // Determine if it's a float or integer
        if (token.value.find('.') != string::npos && token.value.find('f') != string::npos) {
            token.type = FLOAT_SINGLE;
        } else if (token.value.find('.') != string::npos && token.value.find('f') == string::npos) {
            token.type = FLOAT_DOUBLE;
        }
        return token;
    }

    // Check for operators
    if (isOperator(line[pos])) {
        token.type = OPERATOR;
        token.value += line[pos++];
        return token;
    }

    // Default signs
    if (line[pos] == '(' || line[pos] == ')' || line[pos] == '[' || line[pos] == ']' || line[pos] == ';') {
        token.type = DEFAULT_SIGN;
        token.value += line[pos++];
        return token;
    }

    // Unknown token
    token.type = WHITESPACE; // Assuming unknown characters are whitespaces
    token.value += line[pos++];
    return token;
}

int main() {
    string line;
    cout << "Enter the code (terminate with an empty line):" << endl;
    int lineNumber = 1;

    while (getline(cin, line) && !line.empty()) {
        std::cout<<std::endl;
        size_t pos = 0;
        while (pos < line.size()) {
            Token token = getNextToken(line, pos, lineNumber);
            if (token.type != WHITESPACE && token.type != LINE_COMMENT && token.type != SECTION_COMMENT) {
                switch (token.type) {
                    case KEYWORD:
                        cout << "Keyword";
                        break;
                    case IDENTIFIER:
                        cout << "Identifier";
                        break;
                    case INTEGER:
                        cout << "Integer";
                        break;
                    case FLOAT_SINGLE:
                        cout << "Float Literal (Single Precision)";
                        break;
                    case FLOAT_DOUBLE:
                        cout << "Double Literal (Double Precision)";
                        break;
                    case DEFAULT_SIGN:
                        cout << "Default Sign";
                        break;
                    case OPERATOR:
                        cout << "Operator";
                        break;
                }
                cout << " (" << token.value << ") - Line " << token.line << ", Column " << token.column << endl;
            }
        }
        lineNumber++;
    }

    return 0;
}
