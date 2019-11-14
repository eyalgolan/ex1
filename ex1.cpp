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
  regex stringPattern("([a-zA-Z]+[_a-zA-Z0-9]*=[0-9].?[0-9]+[0-9]*;)+");
  if (regex_match(input, stringPattern)) {
    return true;
  }
  return false;
}
void Interpreter::setVariables (string input) {
  //if (!varValidation(input)) {
  //  throw runtime_error("incorrect input");
  //}
  string word;
  string left;
  string right;
  string varName;
  bool isExp = false;
  for (int i = 0; i < input.length(); i++) {
    if (input[i] == ';') {
      for (int j = 0; j < word.length(); j++) {
        if (word[j] == '=') {
          left = varName;
          varName = "";
          isExp = true;
        }
        else if (isExp) {
          right = right + word[j];
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
    for (int j = 0; j < word.length(); j++) {
      if (word[j] == '=') {
        left = varName;
        varName = "";
        isExp = true;
      }
      else if (isExp) {
        right = right + word[j];
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

int Interpreter::precidense(char curr) {
  int pr = -1;
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
deque<char> Interpreter::convertInfixToPostfix (string input) {
  stack <char> opStack;
  deque <char> valQueue;

  for(int i=0; i<input.length(); i++) {
    char curr = input[i];
    if(isdigit(curr)) {
      valQueue.push_back(curr);
    }
    else if(curr == '+' || curr == '-' || curr == '*' || curr == '/') {
      if((curr=='+' || curr=='-') && i==0) {
        if(curr=='+') {
          curr='$';
        }
        else {
          curr='#';
        }
      }
      else if ((curr=='+' || curr=='-') && input[i-1]=='(') {
        if(curr=='+') {
          curr='$';
        }
        else {
          curr='#';
        }
      }
      while(!opStack.empty() && precidense(curr)<precidense(opStack.top())) {
        valQueue.push_back(opStack.top());
        opStack.pop();
      }
      opStack.push(curr);
    }
    if(curr == '(') {
      opStack.push(curr);
    }
    if(curr == ')') {
      while(opStack.top()!='(') {
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

  while(!valQueue.empty()) {
    cout << valQueue.front() << " ";
    valQueue.pop_front();
  }

  return valQueue;
}

string Interpreter::convertVarToValue(string input) {
  int varCount=0;
  string varBuffer[this->inputs.size()];
  for(int i=0; i<input.length(); i++) {
    if((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z')) {
      while(input[i] != '+' && input[i] != '-' && input[i] != '*' && input[i] != '/' && input[i] != '(' && input[i] != ')') {
        varBuffer[varCount] += input[i];
        i++;
      }
      varCount++;
    }
  }

  for(int j=0 ; j < varCount ; j++) {
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
  string returnString = input;
  return returnString;
}

Expression* Interpreter::buildExp(deque <char> postfix) {
  stack<Expression*> expStack;
  char curr = postfix.front();
  string currStr(1,curr);
  while(!postfix.empty() && isdigit(currStr[0])) {
    double var = stod(currStr);
    expStack.push(new Value(var));
    postfix.pop_front();
    currStr = postfix.front();
  }

  while(!postfix.empty()) {
    if (currStr == "+" || currStr == "-" || currStr == "*" || currStr == "/") {
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
    postfix.pop_front();
    currStr = postfix.front();
  }
  return expStack.top();
}
Expression* Interpreter::interpret(string input){
  //----------------replace variables by numbers-------------------------//
  int varCount=0;
  string varBuffer[this->inputs.size()];
  for(int i=0; i<input.length(); i++) {
    if((input[i] >= 'a' && input[i] <= 'z') || (input[i] >= 'A' && input[i] <= 'Z')) {
      while(input[i] != '+' && input[i] != '-' && input[i] != '*' && input[i] != '/' && input[i] != '(' && input[i] != ')') {
        varBuffer[varCount] += input[i];
        i++;
      }
      varCount++;
    }
  }

  for(int j=0 ; j < varCount ; j++) {
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
  stack <char> opStack;
  deque <char> valQueue;

  for(int i=0; i<input.length(); i++) {
    char curr = input[i];
    if(isdigit(curr)) {
      valQueue.push_back(curr);
    }
    else if(curr == '+' || curr == '-' || curr == '*' || curr == '/') {
      if((curr=='+' || curr=='-') && i==0) {
        if(curr=='+') {
          curr='$';
        }
        else {
          curr='#';
        }
      }
      else if ((curr=='+' || curr=='-') && input[i-1]=='(') {
        if(curr=='+') {
          curr='$';
        }
        else {
          curr='#';
        }
      }
      while(!opStack.empty() && precidense(curr)<precidense(opStack.top())) {
        valQueue.push_back(opStack.top());
        opStack.pop();
      }
      opStack.push(curr);
    }
    if(curr == '(') {
      opStack.push(curr);
    }
    if(curr == ')') {
      while(opStack.top()!='(') {
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
    throw "Math Error: attempted divide by 0";
  }
  //otherwise calculate and return
  return this->getLeft()->calculate() / this->getRight()->calculate();
}