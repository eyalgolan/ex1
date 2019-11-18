//
// Created by eyal on 07/11/19.
//

#include <iostream>
#include <stdexcept>
#include "ex1.h"
#include "Expression.h"
#include "string"
#include <map>
#include "regex"
#include <stack>
#include <queue>
#include <stdlib.h>

using namespace std;

bool Interpreter::varValidation(string input) {
  string curr;
  string next;
  if(input[0] == ';') {
    return false;
  }
  if(input.find('=') == string::npos) {
    return false;
  }
  for(unsigned int i=0; i<input.length(); i++) {
    curr = input[i];
    char currChar = curr[0];
    if(!(isdigit(currChar) || (currChar >= 'a' && currChar <= 'z') ||
    (currChar >= 'A' && currChar <= 'Z') || (currChar == '_') ||
    (currChar == '.') || (currChar == '=') || (currChar == ';'))) {
      return false;
    }
    if(i+1<input.length()){
      next = input[i+1];
      if(curr == "/" || curr == "*" || curr == "(" || curr == ")") {
        return false;
      }
      if((curr == "=" || curr == ";" || curr == ".") && (next == "=" || next == ";" || next == ".")) {
        return false;
      }
    }
  }
  return true;
}

bool Interpreter::varNameValidation(string var) {
  char curr;
  for (unsigned int i = 0; i < var.length(); i++) {
    curr = var[i];
    if(i==0 && isdigit(curr)) {
      return false;
    }
    if(curr == '.'){
      return false;
    }
    if(!(isdigit(curr) || (curr >= 'a' && curr <= 'z') || (curr >= 'A' && curr <= 'Z') || (curr == '_'))) {
      return false;
    }
  }
  return true;
}
void Interpreter::setVariables (string input) {
  if (!varValidation(input)) {
    throw "illegal variable assignment!";
  }
  string word;
  string left;
  string right;
  string varName;
  bool isExp = false;
  for (unsigned int i = 0; i < input.length(); i++) {
    if (input[i] == ';') {
      for (unsigned int j = 0; j < word.length(); j++) {
        if (word[j] == '=') {
          left = varName;
          if(!varNameValidation(left)) {
            throw "invalid input";
          }
          varName = "";
          isExp = true;
        }
        else if (isExp) {
          if(!(isdigit(word[j]) || word[j] == '.')) {
            throw "invalid input";
          }
          else {
            right = right + word[j];
          }
        }
        else {
          varName = varName + word[j];
        }
      }
      this->inputs.insert(pair<string, string>(left, right));
      isExp = false;
      word = "";
      right="";
    }
    else {
      word = word + input[i];
    }
  }
  if(input[input.length()-1] != ';') {
    for (unsigned int j = 0; j < word.length(); j++) {
      if (word[j] == '=') {
        left = varName;
        if(!varNameValidation(left)) {
          throw "invalid input";
        }
        varName = "";
        isExp = true;
      }
      else if (isExp) {
        if(!(isdigit(word[j]) || word[j] == '.')) {
          throw "invalid input";
        }
        else {
          right = right + word[j];
        }
      }
      else {
        varName = varName + word[j];
      }
    }
    map<string,string>::iterator repeatedVar = inputs.find(left);
    if(repeatedVar != inputs.end()){
      repeatedVar->second = right;
    }
    else {
      this->inputs.insert(pair<string, string>(left, right));
    }
  }
}

int Interpreter::precidense(string currStr) {
  int pr = -1;
  char curr = currStr[0];
  switch(curr) {
    default: break;
    case '+': pr=1; break;
    case '-': pr=1; break;
    case '*': pr=2; break;
    case '/': pr=2; break;
    case '$': pr=3; break;
    case '#': pr=3; break;
  }
  return pr;
}

Expression* Interpreter::buildExp(deque <string> postfix) {
  stack<Expression*> expStack;
  string currStr;// = postfix.front();
  //string currStr(1,curr);
  while(!postfix.empty()) {
    currStr = postfix.front();
    postfix.pop_front();
    if(!isOperator(currStr)) {
      double var = stod(currStr);
      expStack.push(new Value(var));
    }
    else if (currStr == "+" || currStr == "-" || currStr == "*" || currStr == "/") {
      Expression *right = expStack.top();
      expStack.pop();
      Expression *left = expStack.top();
      expStack.pop();
      if (currStr == "+") {
        Expression *plus = new Plus(left, right);
        expStack.push(plus);
      } else if (currStr == "-") {
        Expression *minus = new Minus(left, right);
        expStack.push(minus);
      } else if (currStr == "*") {
        Expression *mul = new Mul(left, right);
        expStack.push(mul);
      } else if (currStr == "/") {
        Expression *div = new Div(left, right);
        expStack.push(div);
      }
    } else if (currStr == "$" || currStr == "#") {
      Expression *one = expStack.top();
      expStack.pop();
      if (currStr == "$") {
        Expression *uplus = new UPlus(one);
        expStack.push(uplus);
      } else {
        Expression *uminus = new UMinus(one);
        expStack.push(uminus);
      }
    }
  }
  return expStack.top();
}
bool Interpreter::isOperator(string op) {
  if(op == "+" || op == "-" || op == "*" || op == "/" || op == "$" || op == "#" || op == "(" || op == ")") {
    return true;
  }
  return false;
}
bool Interpreter::checkBrackets(string input) {
  stack <char> brackets;
  for(unsigned int i=0; i<input.length(); i++) {
    char currChar = input[i];
    if(currChar == '(') {
      brackets.push(currChar);
    }
    else if(currChar == ')') {
      if(!brackets.empty()) {
        brackets.pop();
      }
      else {
        return false;
      }
    }
  }
  if(!brackets.empty()) {
    return false;
  }
  return true;
}
bool Interpreter::checkOperators(string input) {
  string curr;
  string next;
  for(unsigned int i=0; i<input.length(); i++) {
    curr = input[i];
    if(i+1<input.length()){
      next = input[i+1];
      if((curr == "+" || curr == "-" || curr == "/" || curr == "*" || curr == ".") &&
          (next == "+" || next == "-" || next == "/" || next == "*" || next == ".")) {
        return false;
      }
      if(curr == "-" && next == ")") {
        return false;
      }
      if(curr == "(" && next == ")") {
        return false;
      }
    }
  }
  return true;
}
Expression* Interpreter::interpret(string input){
  //check brackets validity
  bool vaildBrackets = checkBrackets(input);
  if (!vaildBrackets) {
    throw "invalid brackets";
  }
  //check operator validity (no *+ etc and no ..)
  bool validOperators = checkOperators(input);
  if (!validOperators) {
    throw "illegal math expression";
  }
  //----------------replace variables by numbers--convertVarToValue-----------------------//
  unsigned int varCount=0;
  unsigned int bufferSize = this->inputs.size();
  string* varBuffer = new string[bufferSize];
  for(unsigned int i=0; i<input.length(); i++) {
    if(i + 1 < input.length()) {
      if ((input[i] == '-' || input[i] =='+') && ((input[i+1] >= 'a' && input[i+1] <= 'z') || (input[i+1] >= 'A' && input[i+1] <= 'Z'))) {
        throw "invalid input";
      }
    }
    if((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z') || (input[i] == '_')) {
      while(input[i] != '+' && input[i] != '-' && input[i] != '*' && input[i] != '/' && input[i] != '(' && input[i] != ')' && i<input.length()) {
        varBuffer[varCount] += input[i];
        i++;
      }
      varCount++;
    }
  }

  for(unsigned int j=0 ; j < varCount ; j++) {
    if(this->inputs.find(varBuffer[j])==inputs.end()) {
      throw "no such variable";
    }
    else {
      string var = inputs.find(varBuffer[j])->first;
      string value = inputs.find(varBuffer[j])->second;
      size_t pos = input.find(var);
      input.replace(pos, var.length(), value);
    }
  }


  //-----------------------infix to postfix--------------------//

  stack <string> opStack;
  deque <string> valQueue;
  for(unsigned int i=0; i<input.length(); i++) {
    string buffer = "";
    char currChar = input[i];
    string currStr(1, currChar);
    bool operandDetected = false;
    while (!isOperator(currStr) && i < input.length()) {
      operandDetected = true;
      buffer += currStr;
      i++;
      currChar = input[i];
      currStr = currChar;
    }
    if(operandDetected) {
      valQueue.push_back(buffer);
      operandDetected = false;
    }
    if(currStr == "+" || currStr == "-" || currStr == "*" || currStr == "/") {
      if((currStr=="+" || currStr=="-") && i==0) {
        if(currStr=="+") {
          currStr="$";
        }
        else {
          currStr="#";
        }
      }
      else if ((currStr=="+" || currStr=="-") && input[i-1]=='(') {
        if(currStr =="+") {
          currStr="$";
        }
        else {
          currStr="#";
        }
      }
      while(!opStack.empty() && precidense(currStr)<precidense(opStack.top())) {
        valQueue.push_back(opStack.top());
        opStack.pop();
      }
      opStack.push(currStr);
    }
    if(currStr == "(") {
      opStack.push(currStr);
    }
    if(currStr == ")") {
      while(opStack.top()!="(") {
        valQueue.push_back(opStack.top());
        opStack.pop();
      }
      opStack.pop();
    }
  }
  while(!opStack.empty()) {
    valQueue.push_back(opStack.top());
    opStack.pop();
  }

  //while(!valQueue.empty()) {
  //  cout << valQueue.front() << " ";
  //  valQueue.pop_front();
  //}

  Expression* e = buildExp(valQueue);
  return e;
}



double Value::calculate() {
  return this->number;
}

double Variable::calculate() {
  return this->value;
}
Variable& Variable::operator++() {
  this->value += 1;
  return *this;
}
Variable& Variable::operator--() {
  this->value -= 1;
  return *this;
}
Variable& Variable::operator+=(const Variable& toAdd) {
  this->value += toAdd.value;
  return *this;
}
Variable& Variable::operator-=(const Variable& toReduce) {
  this->value -= toReduce.value;
  return *this;
}
//todo check if this works
Variable& Variable::operator++(int) {
  this->value += 1;
  return *this;
}
//todo check if this works
Variable& Variable::operator--(int) {
  this->value -= 1;
  return *this;
}

Expression* UnaryOperation::getExp() {
  return this->exp;
}
void UnaryOperation::setExp(Expression* e) {
  this->exp = e;
}

double UPlus::calculate() {
  return this->getExp()->calculate();
}

double UMinus::calculate() {
  return -(this->getExp()->calculate());
}

Expression* BinaryOperation::getRight() {
  return this->right;
}
Expression* BinaryOperation::getLeft() {
  return this->left;
}
void BinaryOperation::setRight(Expression* r) {
  this->right = r;
}
void BinaryOperation::setLeft(Expression* l) {
  this->left = l;
}

double Plus::calculate() {
  return this->getLeft()->calculate() + this->getRight()->calculate();
}

double Minus::calculate() {
  return this->getLeft()->calculate() - this->getRight()->calculate();
}

double Mul::calculate() {
  return this->getLeft()->calculate() * this->getRight()->calculate();
}

double Div::calculate() {
  //if denominator is 0, throw error
  if (this->getRight()->calculate() == 0) {
    throw "division by zero";
  }
  //otherwise calculate and return
  return this->getLeft()->calculate() / this->getRight()->calculate();
}