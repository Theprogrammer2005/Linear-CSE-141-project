
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

int main() {
    string equation;
    getline(cin, equation); // e.g. "3x2+2x1+4x3=16"

    // Separate into LHS and RHS
    int equalPos = equation.find('='); // tries to find = sign
    string lhs = equation.substr(0, equalPos); //naming a lhs variable
    string rhs = equation.substr(equalPos + 1); //naming a rhs variable

    // We'll store coefficients in an array
    int coeffs[100] = {0}; // Assuming max variable index = 99

    // Replace '-' with '+-' for easy splitting
    for (int i = 0; i < lhs.size(); i++) {
        if (lhs[i] == '-') {
            lhs.insert(i, "+");
            i++;
            }

        }

    // Parse each term
    stringstream ss(lhs);
    string term;
    while (getline(ss, term, '+')) {
        if (term.empty()) continue;

        int coeff = 0, varIndex = 0;
        char x;
        stringstream ts(term);

        // Handle missing coefficient (e.g. "x1" means 1x1)
        if (isdigit(term[0]) || term[0] == '-') {
            ts >> coeff >> x >> varIndex;
        } else {
            coeff = 1; // default
            ts >> x >> varIndex;
        }

        coeffs[varIndex] += coeff; // sum in case of duplicates
    }

    // Print sorted equation
    bool firstTerm = true;
    for (int i = 1; i < 100; i++) {
        if (coeffs[i] != 0) {
            if (!firstTerm && coeffs[i] > 0) cout << "+";
            cout << coeffs[i] << "x" << i;
            firstTerm = false;
        }
    }
    cout << "=" << rhs << endl;

    return 0;
}
