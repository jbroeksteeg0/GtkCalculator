#include <iostream>
#include <set>

using namespace std;

extern set<int> operations;
string evaluate(string input);
bool isNumber(string input);
bool isOperator(char ch);
bool isOperator(string s);