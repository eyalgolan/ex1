//
// Created by eyal on 07/11/19.
//

#ifndef EX1_EX1_H
#define EX1_EX1_H

#include "Expression.h"
#include "string"
#include <map>
#include <queue>

class Interpreter {
 private:
  map<string, string> inputs;
  bool varValidation (string input);
  queue<char> convertInfixToPostfix(string input);
  string queueToString(queue<char>postfix);
  string convertVarToValue(string input);
 public:
  Interpreter(){};
  ~Interpreter(){};
  void setVariables (string input);
  Expression* interpret (string input);
};

class Value : public Expression{
 private:
  double number;
 public:
  explicit Value(double number) : number(number){}
  virtual ~Value(){}
  double calculate() override;
};

class Variable : public Expression {
 private:
  string name;
  double value;
 public:
  Variable(string name, double value) : name(name), value(value) {}
  virtual ~Variable(){}
  double calculate() override;
  Variable& operator++();
  Variable& operator--();
  Variable& operator+=(const Variable& toAdd);
  Variable& operator-=(const Variable& toReduce);
  //todo check if this works
  Variable& operator++(int);
  //todo check if this works
  Variable& operator--(int);
};

class UnaryOperation : public Expression {
 protected:
  Expression* exp {nullptr};
 public:
  explicit UnaryOperation(Expression* exp) : exp(exp) {}
  virtual ~UnaryOperation(){}
  Expression* getExp();
  void setExp(Expression* e);
};

class UPlus : public UnaryOperation {
 public:
  explicit UPlus(Expression* exp) : UnaryOperation(exp) {}
  virtual ~UPlus(){}
  double calculate() override;
};

class UMinus : public UnaryOperation {
 public:
  explicit UMinus(Expression* exp) : UnaryOperation(exp) {}
  virtual ~UMinus(){}
  double calculate() override;
};

class BinaryOperation : public Expression {
 protected:
  Expression* right {nullptr};
  Expression* left {nullptr};

 public:
  BinaryOperation(Expression* left, Expression* right) : left(left), right(right) {}
  virtual ~BinaryOperation(){}
  Expression* getRight();
  Expression* getLeft();
  void setRight(Expression* r);
  void setLeft(Expression* l);
};

class Plus : public BinaryOperation{
 public:
  Plus(Expression* left, Expression* right) : BinaryOperation(left, right){}
  virtual ~Plus(){}
  double calculate() override;
};

class Minus : public BinaryOperation{
 public:
  Minus(Expression* left, Expression* right) : BinaryOperation(left, right){}
  virtual ~Minus(){}
  double calculate() override;
};

class Mul : public BinaryOperation{
 public:
  Mul(Expression* left, Expression* right) : BinaryOperation(left, right){}
  virtual ~Mul(){}
  double calculate() override;
};

class Div : public BinaryOperation{
 public:
  Div(Expression* left, Expression* right) : BinaryOperation(left, right){}
  virtual ~Div(){}
  double calculate() override;
};
#endif //EX1_EX1_H