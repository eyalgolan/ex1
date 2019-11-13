//
// Created by eyal on 07/11/19.
//

#include <stdexcept>
#include "ex1.h"
#include "Expression.h"
#include "string"
#include <map>
#include "regex"
#include <stack>
#include <queue>

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
      this->inputs.insert(pair<string, double>(left, stod(right)));
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
    this->inputs.insert(pair<string, double>(left, stod(right)));
  }

}

Expression* Interpreter::interpret(string input){
  stack <char> opStack;
  queue <char> valQueue;
  queue <char> brackets;

  for (int i = 0; i < input.length(); i++) {
    //check if input is operator
    if(input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/' || input[i] == '(' || input[i] == ')' || input[i]=='.') {

      //check for order of operators
      if ((opStack.top()=='*' || opStack.top() == '/') && (input[i] == '+' || input[i] == '-'))
      {
        valQueue.push(opStack.top());
      }

      //check legal brackets order
      else if(input[i] == '(') {
        brackets.push(input[i]);
      }
      else if(input[i] == ')') {
        if(brackets.empty()){
          throw runtime_error("illegal order of brackets");
        }
        else {
          brackets.pop();
          while(!opStack.empty() || opStack.top() == '(') {
            valQueue.push(opStack.top());
          }
          //pop last (
          if(opStack.top() == '(') {
            opStack.pop();
          }
        }
      }

      //handle operands
      else if (isdigit(input[i])){
        opStack.push(input[i]);
      }
      else {
        throw runtime_error("not a valid operator or operand");
      }
    }
    //handle operands
    else {
      valQueue.push(input[i]);
    }
  }
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
  //if denominator is 0, throw runtime error
  if (this->getRight()->calculate() == 0) {
    throw runtime_error("Math Error: attempted divide by 0");
  }
  //otherwise calculate and return
  return this->getLeft()->calculate() / this->getRight()->calculate();
}