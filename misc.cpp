#include <iostream>
#include <set>

using namespace std;

set<int> operations = {
    '+',
    '-',
    '*',
    '/',
    '^'
};

bool isNumber(string input) {
    bool foundDecimal = false;
    for (int i = 0; i < input.length(); i++) {
        if (!isdigit(input[i])) {
            if (input[i] == '.' && !foundDecimal) {
                foundDecimal = true;
            } else {
                return false;
            }
        }
    }
    return true;
}

bool isOperator(char ch) {return operations.count(ch); }
bool isOperator(string s) {return s.length() == 1 && operations.count( s[0] ); }