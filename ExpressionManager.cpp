#include "ExpressionManager.h"
#include <sstream>

using namespace std;

// public functions

int ExpressionManager::value(){
    stack<int> operands;
    stack<string> operators;
    string infixString = infix();
    ParseType parseType = parse(expression);
    while(parseType != endline){
        if(parseType == num){  //if the character is an operand add it to the string
            operands.push(stoi(parseVal));
        }
        else if(parseType == operate){  //check if list is empty and compare precedence to top of stack
            while(!operators.empty() && postfix_precedence(parseVal) <= postfix_precedence(operators.top())){
                evaluate(operands, operators);
            }
            operators.push(parseVal);
        }
        else if(parseType == opener){  //push: ( [ or { onto stack
            operators.push(parseVal);
        }
        else if(parseType == closer){  //pop and output stack until the closer is found
            while(operators.top() != "(" && operators.top()
                                             != "[" && operators.top() != "{"){
                evaluate(operands, operators);
            }
            operators.pop();
        }
        else if(parseVal == " ") {}  //if space do nothing
        parseType = parse(expression);
    }
    while(!operators.empty()){  ////pop all elements from the stack
        evaluate(operands, operators);
    }
    resetIndex();
    return operands.top();
}
string ExpressionManager::infix(){
    if(isBalanced() == false) {
        return "Unbalanced";
    }
    string infixString;
    ParseType parseType = parse(expression);
    while(parseType != endline){
        if(parseType == illegal){  //throws error if the operator is illegal
            return "Illegal Operator";
        }
        infixString = infixString + parseVal + " ";
        if(parseType == num){  //throws error if two numbers are in a row
            parseType = parse(expression);
            if(parseType == num){
                return "Missing Operator";
            }
        }
        else if(parseType == operate){
            parseType = parse(expression);
            if(parseType == operate || parseType == endline){  //throws error if two operators are in a row
                return "Missing Operand";
            }
        }
        else{
            parseType = parse(expression);
        }
    }
    resetIndex();
    return infixString;
}
string ExpressionManager::postfix(){
    if(isBalanced() == false) {
        return "Unbalanced";
    }
    stack<string> stack;
    string postfixString;
    string infixString = infix();
    ParseType parseType = parse(expression);
    while(parseType != endline){
        if(parseType == num){  //add operand to output
            postfixString = postfixString + parseVal + " ";
        }
        else if(parseType == operate){  //check if list is empty and compare precedence to top of stack
            while(!stack.empty() && postfix_precedence(parseVal) <= postfix_precedence(stack.top())){
                postfixString = postfixString + stack.top() + " ";
                stack.pop();
            }
            stack.push(parseVal);
        }
        else if(parseType == opener){  //push: ( [ or { onto stack is found
            stack.push(parseVal);
        }
        else if(parseType == closer){  //pop and output stack until the closer
            while(stack.top() != "(" && stack.top() != "[" && stack.top() != "{"){
                postfixString = postfixString + stack.top() + " ";
                stack.pop();
            }
            stack.pop();
        }
        else if(parseVal == " ") {}  //if space do nothing
        parseType = parse(expression);
    }
    while(!stack.empty()){  //pop all elements left of stack
        postfixString = postfixString + stack.top() + " ";
        stack.pop();
    }
    resetIndex();
    return postfixString;
}
string ExpressionManager::prefix(){
    if(isBalanced() == false) {
        return "Unbalanced";
    }
    string str = infix();
    reverse(str.begin(), str.end());
    for(int i = 0; i < str.size(); i++){  //replace braces with their opposite brace
        if(str[i] == '('){
            str[i] = ')';
            i++;
        }
        else if(str[i] == ')'){
            str[i] = '(';
            i++;
        }
        if(str[i] == '['){
            str[i] = ']';
            i++;
        }
        else if(str[i] == ']'){
            str[i] = '[';
            i++;
        }
        if(str[i] == '{'){
            str[i] = '}';
            i++;
        }
        else if(str[i] == '}'){
            str[i] = '{';
            i++;
        }
    }
    string prefixString = prefixPrep(str);
    reverse(prefixString.begin(), prefixString.end());
    string returnString = prefixString.substr(1, prefixString.size() - 1) + " "; //fixes any whitespace
    return returnString;
}
string ExpressionManager::toString() const{
    ostringstream os;
    os << expression;
    return os.str();
}

// private functions

ExpressionManager::ParseType ExpressionManager::parse(string str){
    parseVal = "";
    while(index != str.size()){
        switch(str.at(index)){
            case '(':
            case '[':
            case '{':
                parseVal = str.at(index);
                index++;
                return opener;
            case ')':
            case ']':
            case '}':
                parseVal = str.at(index);
                index++;
                return closer;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                parseVal += str.at(index);
                break;
            case '*':
            case '/':
            case '%':
            case '+':
            case '-':
                parseVal = str.at(index);
                index++;
                return operate;
            case ' ':
                break;
            default:
                index++;
                return illegal;
        }
        index++;
        if(parseVal.size() > 0){
            if(index == str.size() || (index < str.size() && (str.at(index) < '0' || str.at(index) > '9'))){
                return num;
            }
        }
    }
    return endline;
}

string ExpressionManager::prefixPrep(string str){
    if(isBalanced() == false) {
        return "Unbalanced";
    }
    str = "(" + str + ")";
    stack<string> stack;
    string outputString;

    ParseType parseType = parse(str);
    while(parseType != endline){
        if(parseType == num) {  //add operand to string
            outputString = outputString + parseVal + " ";
        }
        else if(parseType == operate){
            while(!stack.empty() && prefix_precedence(parseVal) < prefix_precedence(stack.top())){
                outputString = outputString + stack.top() + " ";
                stack.pop();
            }
            stack.push(parseVal);
        }
        else if(parseType == opener){  //push: ( [ or { onto stack
            stack.push(parseVal);
        }
        else if(parseType == closer){  //pop and output stack until the closer is found
            while(stack.top() != "(" && stack.top() != "[" && stack.top() != "{"){
                outputString = outputString + stack.top() + " ";
                stack.pop();
            }
            stack.pop();  //remove opener from the stack
        }
        else if(parseVal == " ") {}  //if space do nothing
        parseType = parse(str);
    }
    while(!stack.empty()){
        outputString = outputString + stack.top() + " ";
        stack.pop();
    }
    resetIndex();
    return outputString;
}
void ExpressionManager::evaluate(stack<int>&operands, stack<string>&operators){
    int a, b, c;
    b = operands.top();
    operands.pop();
    a = operands.top();
    operands.pop();
    switch(operators.top().at(0)){
        case '+':
            c = a + b;
            break;
        case '-':
            c = a - b;
            break;
        case '*':
            c = a * b;
            break;
        case '/':
            c = a / b;
            break;
        case '%':
            c = a % b;
            break;
    }
    operands.push(c);
    operators.pop();
}
void ExpressionManager::resetIndex(){
    index = 0;
}
bool ExpressionManager::isBalanced(){
    string expr = "[ " + expression + " ]";
    stack<char> s;
    char x;
    for(int i = 0; i < expr.length(); i++){
        if(expr[i] == '(' || expr[i] == '[' || expr[i] == '{'){
            s.push(expr[i]);
            continue;
        }
        if(s.empty()) {
            return false;
        }
        switch(expr[i]) {
            case ')':
                x = s.top();
                s.pop();
                if(x == '{' || x == '[')
                    return false;
                break;
            case '}':
                x = s.top();
                s.pop();
                if(x == '(' || x == '[')
                    return false;
                break;
            case ']':
                x = s.top();
                s.pop();
                if(x == '(' || x == '{')
                    return false;
                break;
        }
    }
    return(s.empty());
}
int ExpressionManager::postfix_precedence(string str){
    if(str == "/" || str == "*" || str == "%"){
        return 2;
    }
    else if(str == "+" || str == "-") {
        return 1;
    }
    else{
        return -1;
    }
}
int ExpressionManager::prefix_precedence(string str){
    if(str == "-" || str == "+") {
        return 1;
    }
    else if(str == "*" || str == "/" || str == "%") {
        return 2;
    }
    else{
        return 0;
    }
}

