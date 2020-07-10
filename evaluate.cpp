#include <iostream>
#include <list>
#include <vector>
#include <set>
#include <stack>
#include <cassert>
#include <math.h>

#include "eval.h"
using namespace std;

// return a string of either the answer, or an error

enum Type {
    NUMBER,
    OPERATOR,
    FUNCTION,
    EXPRESSION
};
struct Token {
    Type type;
    string value;
    list<Token> parts;
};

string tokenEval(list<Token> &currentList) {
    // eval brackets

    long double ans = 0;

    auto iter = currentList.begin();

    // evaluate brackets
    while (iter != currentList.end()) {
        if (iter->type == EXPRESSION) {
            string res = tokenEval(iter->parts);
            if (res=="ERROR") {
                return res;
            }
            *iter = Token({NUMBER, res});
        }
        iter++;
    }

    // evaluate negative numbers
    iter = currentList.begin();
    while (iter != currentList.end()) {
        if (iter->type == OPERATOR && iter->value == "-" && (iter == currentList.begin() || prev(iter)->type != NUMBER)) {
            assert(next(iter) != currentList.end() && next(iter)->type == NUMBER);
            *iter = Token({NUMBER, to_string(stold(next(iter)->value)*-1)});
            currentList.erase(next(iter));
        }
        iter++;
    }

    // evaluate functions
    iter = currentList.begin();
    while (iter != currentList.end()) {
        if (iter->type == FUNCTION) {
            if (iter->value == "log2") {
                if (next(iter) == currentList.end() || next(iter)->type != NUMBER) {
                    cout << "ERROR: LOG2 ARGUMENTS" << endl;
                    return "ERROR";
                }
                long double after = stold(next(iter)->value);
                *iter = Token({NUMBER, to_string(log2l(after))});
                currentList.erase(next(iter));
            }
            if (iter->value == "sqrt") {
                if (next(iter) == currentList.end() || next(iter)->type != NUMBER) {
                    cout << "ERROR: SQRT ARGUMENTS" << endl;
                    return "ERROR";
                }
                long double after = stold(next(iter)->value);
                *iter = Token({NUMBER, to_string(sqrtl(after))});
                currentList.erase(next(iter));
            }
        }
        iter++;
    }
    // evaluate powers
    iter = currentList.begin();
    while (iter !=currentList.end()) {
        if (iter->type == OPERATOR && iter->value == "^") {
            if (iter != currentList.begin() && next(iter) != currentList.end() && prev(iter)->type == NUMBER && next(iter)->type == NUMBER) {
                *iter = Token({NUMBER, to_string( pow(stold(prev(iter)->value), stold(next(iter)->value)) )});
                currentList.erase(prev(iter));
                currentList.erase(next(iter));
            } else {
                // cout << "ERROR: ^ MUST HAVE NUMBER BEFORE AND AFTER" << endl;
                return "ERROR";
            }
        }
        iter++;
    }

    // evaluate multipliation, division, modulo

    iter = currentList.begin();
    while (iter !=currentList.end()) {
        if (iter->type == OPERATOR && (iter->value == "*" || iter->value == "/" || iter->value == "%")) {
            if (iter != currentList.begin() && next(iter) != currentList.end() && prev(iter)->type == NUMBER && next(iter)->type == NUMBER) {
                
                long double before = stold(prev(iter)->value), after = stold(next(iter)->value);

                if (iter->value == "*") {*iter = Token({NUMBER,to_string(before*after)});}
                if (iter->value == "/") {
                    if (after==0.0) {
                        // cout << "ERROR: DIVISION BY ZERO" << endl;
                        return "ERROR";
                    }
                    *iter = Token({NUMBER,to_string(before/after)});
                }
                if (iter->value == "%") {
                    if (after==0.0) {
                        // cout << "ERROR: MODULO BY ZERO" << endl;
                        return "ERROR";
                    }
                    *iter = Token({NUMBER,to_string(fmod(before,after))});
                }

                currentList.erase(prev(iter));
                currentList.erase(next(iter));
            } else {
                // cout << "ERROR: " << iter->value << " MUST HAVE NUMBER BEFORE AND AFTER" << endl;
                return "ERROR";
            }
        }
        iter++;
    }

    // addition and subtraction
    iter = currentList.begin();
    while (iter !=currentList.end()) {
        if (iter->type == OPERATOR && (iter->value == "+" || iter->value == "-")) {
            if (iter != currentList.begin() && next(iter) != currentList.end() && prev(iter)->type == NUMBER && next(iter)->type == NUMBER) {
                long double before = stold(prev(iter)->value);
                long double after = stold(next(iter)->value);
                long double newVal;
                if (iter->value == "+") {newVal = before+after;}
                if (iter->value == "-") {newVal = before-after;}

                *iter = Token({NUMBER, to_string(newVal)});
                currentList.erase(prev(iter));
                currentList.erase(next(iter));
            } else {
                // cout << "ERROR: " << iter->value << " MUST HAVE NUMBER BEFORE AND AFTER" << endl;
                return "ERROR";
            }
        }
        iter++;
    }
    
    // assert( currentList.size() == 1 );
    if (currentList.size() != 1) {
        return "ERROR";
    }
    auto x = currentList.begin();
    return currentList.begin()->value;
}

string evaluate(string input) {
    string current = "";

    input = "(" + input + ")";

    // stack<list<Token>> evalList;


    stack<Token*> tokens;

    Token root({EXPRESSION});

    tokens.push(&root);

    for (int i = 0; i < input.size(); i++) {
        if (input[i] == ' ') { continue; }

        if (input[i] == '(') { // start new context

            if (current != "") {
                if (isNumber(current)) {
                    tokens.top()->parts.push_back({NUMBER,current});
                } else if (isOperator(current)) {
                    tokens.top()->parts.push_back({OPERATOR,current});
                } else { // assume that it's a function
                    tokens.top()->parts.push_back({FUNCTION,current});
                }
            }

            current = "";
            // tokens.push_back({BRACKET, "("});
            tokens.top()->parts.push_back({EXPRESSION});
            tokens.push( &(tokens.top()->parts.back()) ); // new top bit

        } else if (input[i] == ')') { // merge back
             if (current != "") {
                if (isNumber(current)) {
                    tokens.top()->parts.push_back({NUMBER,current});
                } else if (isOperator(current)) {
                    tokens.top()->parts.push_back({OPERATOR,current});
                } else { // assume that it's a function
                    tokens.top()->parts.push_back({FUNCTION, current});
                }
            }
            current = "";           
            tokens.pop(); // move down a thing
        } else if (isOperator(input[i])) { // no operations in function names, must be an operation
            if (current != "") { // if necessary, push back previous
                if (!isNumber(current) && current != "-") {
                    cout << "ERROR: OPERATION WITH NO NUMBER PRECEDING IT" << endl;
                    return "ERROR";    
                } else if (current == "-") {
                    tokens.top()->parts.push_back({OPERATOR, "-"});
                } else if (isNumber(current)) {
                    tokens.top()->parts.push_back({NUMBER, current});
                } else {
                    cout << "This shouldn't run" << endl;
                    assert(false);
                }

            }
            string temp = string(1,input[i]);
            tokens.top()->parts.push_back({OPERATOR, temp});
            current = "";

        } else if (isdigit(input[i])) { // in any other case, current should be cleared
            if (current != "" && !isNumber(current) && isOperator(current)) {
                tokens.top()->parts.push_back({OPERATOR, current});
                tokens.top()->parts.push_back({FUNCTION, current});
                current = "";
            }
            current += input[i];
        } else {
            current += input[i];
        }
    }

    cout << "";
    list<Token> &x = root.parts.begin()->parts;
    return tokenEval(x);
}

