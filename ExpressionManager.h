#ifndef EXPRESSIONS_EXPRESSIONMANAGER_H
#define EXPRESSIONS_EXPRESSIONMANAGER_H

#include "ExpressionManagerInterface.h"
#include <string>
#include <stack>

using namespace std;

class ExpressionManager: public ExpressionManagerInterface{
public:
    ExpressionManager(){}
    ExpressionManager(string inputExp){
        expression = inputExp;
    }
    ~ExpressionManager(){}
    int value();  //returns the integer value of the infix expression
    string infix();  //returns the infix items from the expression
    string postfix();  //returns postfix expression from the infix expression
    string prefix();  //returns the prefix expression from the infix expression
    string toString() const;  //returns the expressions in the infix vector
    friend ostream& operator<<(ostream& os, ExpressionManager& expression){
        return os << expression.toString();
    }
private:
    string expression;
    int index = 0;
    enum ParseType {opener, closer, num, operate, endline, illegal};
    string parseVal;
    ParseType parse(string str);
    string prefixPrep(string str);
    void evaluate(stack<int>& operands, stack<string>& operators);
    void resetIndex();
    bool isBalanced();
    int postfix_precedence(string str);
    int prefix_precedence(string str);
};
#endif //EXPRESSIONS_EXPRESSIONMANAGER_H
