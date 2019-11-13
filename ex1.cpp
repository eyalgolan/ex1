//
// Created by eyal on 07/11/19.
//

#include <stdexcept>
#include "ex1.h"
#include "Expression.h"
#include "string"
#include <map>
#include "regex"

using namespace std;

bool Interpreter::varValidation(string input) {
  regex stringPattern("([a-zA-Z]+[_a-zA-Z0-9]*=[0-9].?[0-9]+[0-9]*;)+");
  if (regex_match(input, stringPattern)) {
    return true;
  }
  return false;
}
void Interpreter::setVariables (string input) {
  string word;
  string left;
  string right;
  string varName;
  bool isExp = false;
  for (auto x : input) {
    if (x == ';') {
      for (auto y : word) {
        if ( y == '=') {
          left = varName;
          varName = "";
          isExp = true;
        }
        else if (isExp) {
          right = right + y;
        }
        else {
          varName = varName + y;
        }
      }
      this->inputs.insert(pair<string, double>(left, stod(right)));
      isExp = false;
      word = "";
      right="";
    }
    else {
      word = word + x;
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