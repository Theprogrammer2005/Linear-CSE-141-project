#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <cctype>
#include <algorithm>

using namespace std;

// Helper: Trim spaces
string trim(string s) {
    while (!s.empty() && isspace(s.front())) s.erase(s.begin());
    while (!s.empty() && isspace(s.back())) s.pop_back();
    return s;
}

// Parses a single equation string into coefficients and RHS constant
pair<map<string, int>, int> parseEquation(const string& equation) {
    map<string, int> coeffs;
    int rhs = 0;
    bool rhsFlag = false;
    string token;
    istringstream iss(equation);
    char ch;

    while (iss >> noskipws >> ch) {
        if (ch == '=') {
            if (!token.empty()) {
                token = trim(token);
                if (isalpha(token.back())) token += '1';
                size_t pos = token.find_first_of("abcdefghijklmnopqrstuvwxyz");
                string coeffPart = token.substr(0, pos);
                string varPart = token.substr(pos);

                if (coeffPart == "+" || coeffPart == "") coeffPart = "1";
                else if (coeffPart == "-") coeffPart = "-1";

                coeffs[varPart] += stoi(coeffPart);
                token.clear();
            }
            rhsFlag = true;
            continue;
        }

        if (ch == '+' || ch == '-') {
            if (!token.empty()) {
                token = trim(token);
                if (isalpha(token.back())) token += '1';
                size_t pos = token.find_first_of("abcdefghijklmnopqrstuvwxyz");
                string coeffPart = token.substr(0, pos);
                string varPart = token.substr(pos);

                if (coeffPart == "+" || coeffPart == "") coeffPart = "1";
                else if (coeffPart == "-") coeffPart = "-1";

                coeffs[varPart] += stoi(coeffPart);
                token.clear();
            }
            token = ch;
        } else {
            token += ch;
        }
    }

    if (!token.empty()) {
        token = trim(token);
        if (isalpha(token.back())) token += '1';
        size_t pos = token.find_first_of("abcdefghijklmnopqrstuvwxyz");
        string coeffPart = token.substr(0, pos);
        string varPart = token.substr(pos);

        if (coeffPart == "+" || coeffPart == "") coeffPart = "1";
        else if (coeffPart == "-") coeffPart = "-1";

        coeffs[varPart] += stoi(coeffPart);
    }

    // Move constants to RHS
    for (auto it = coeffs.begin(); it != coeffs.end(); ) {
        if (it->first.find_first_not_of("abcdefghijklmnopqrstuvwxyz") != string::npos) {
            rhs -= it->second;
            it = coeffs.erase(it);
        } else {
            ++it;
        }
    }

    if (rhsFlag) {
        size_t pos = equation.find('=');
        string rhsPart = equation.substr(pos + 1);
        istringstream rhsStream(rhsPart);
        string rhsToken;
        while (rhsStream >> rhsToken) {
            if (isdigit(rhsToken[0]) || rhsToken[0] == '+' || rhsToken[0] == '-') {
                rhs += stoi(rhsToken);
            }
        }
    }

    return {coeffs, rhs};
}

// Print equation in proper form
void printEquation(const map<string, int>& coeffs, int rhs) {
    bool first = true;
    for (const auto& kv : coeffs) {
        int coeff = kv.second;
        string var = kv.first;
        if (coeff == 0) continue;

        if (!first) {
            if (coeff > 0) cout << "+";
        }
        if (abs(coeff) != 1) {
            cout << coeff;
        } else {
            if (coeff == -1) cout << "-";
            else if (coeff == 1 && !first) cout << "";
        }
        cout << var;
        first = false;
    }
    cout << "=" << rhs << endl;
}

int main() {
    int n;
    cout << "Enter number of equations: ";
    cin >> n;
    cin.ignore();

    vector<pair<map<string, int>, int>> equations;

    cout << "Enter the equations:" << endl;
    for (int i = 0; i < n; ++i) {
        string eq;
        getline(cin, eq);
        equations.push_back(parseEquation(eq));
    }

    cout << "\nCommands (print, print xi, print row k, quit):" << endl;
    while (true) {
        string command;
        cout << "> ";
        getline(cin, command);
        istringstream cmdStream(command);
        string cmd;
        cmdStream >> cmd;

        if (cmd == "quit") {
            break;
        } else if (cmd == "print") {
            string arg1, arg2;
            cmdStream >> arg1 >> arg2;

            if (arg1.empty()) {
                // print all
                for (const auto& eq : equations) {
                    printEquation(eq.first, eq.second);
                }
            } else if (arg1.substr(0, 1) == "x") {
                // print xi
                for (const auto& eq : equations) {
                    if (eq.first.count(arg1)) {
                        printEquation(eq.first, eq.second);
                    }
                }
            } else if (arg1 == "row" && !arg2.empty()) {
                int rowIndex = stoi(arg2);
                if (rowIndex >= 1 && rowIndex <= n) {
                    printEquation(equations[rowIndex - 1].first, equations[rowIndex - 1].second);
                } else {
                    cout << "Invalid row number." << endl;
                }
            } else {
                cout << "Invalid command." << endl;
            }
        } else {
            cout << "Unknown command." << endl;
        }
    }

    cout << "Program terminated." << endl;
    return 0;
}
