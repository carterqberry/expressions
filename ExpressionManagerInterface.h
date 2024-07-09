#ifndef EXPRESSIONS_EXPRESSIONMANAGERINTERFACE_H
#define EXPRESSIONS_EXPRESSIONMANAGERINTERFACE_H

#include <string>

using namespace std;

class ExpressionManagerInterface {
public:
    ExpressionManagerInterface(void){}
    virtual ~ExpressionManagerInterface(void){}
    virtual int value(void) = 0;  // returns the integer value from the infix expression
    virtual string infix(void) = 0;  // returns the infix items from the expression
    virtual string postfix(void) = 0;  //returns the postfix expression from the infix expression
    virtual string prefix(void) = 0;  //returns the prefix expression from the infix expression
    virtual string toString(void) const = 0;  // returns the items in the infix vector
};
#endif //EXPRESSIONS_EXPRESSIONMANAGERINTERFACE_H
