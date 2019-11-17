//
// Created by eyal on 07/11/19.
//

#ifndef EX1_EX1_H
#define EX1_EX1_H

#include "Expression.h"
#include "string"
#include <map>
#include <queue>
#include <stack>

class Interpreter {
 private:
  map<string, string> inputs;
  int precidense (string);
  Expression* buildExp(deque <string>);
  bool isOperator(string);
  bool checkBrackets(string);
  bool checkOperators(string input);
  bool varValidation(string input);
  bool varNameValidation(string var);
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
  explicit Value(double n) : number(n){}
  virtual ~Value(){}
  double calculate() override;
};

class Variable : public Expression {
 private:
  string name;
  double value;
 public:
  Variable(string n, double v) : name(n), value(v) {}
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
  explicit UnaryOperation(Expression* e) : exp(e) {}
  virtual ~UnaryOperation(){}
  Expression* getExp();
  void setExp(Expression* e);
};

class UPlus : public UnaryOperation {
 public:
  explicit UPlus(Expression* e) : UnaryOperation(e) {}
  virtual ~UPlus(){}
  double calculate() override;
};

class UMinus : public UnaryOperation {
 public:
  explicit UMinus(Expression* e) : UnaryOperation(e) {}
  virtual ~UMinus(){}
  double calculate() override;
};

class BinaryOperation : public Expression {
 protected:
  Expression* left {nullptr};
  Expression* right {nullptr};

 public:
  BinaryOperation(Expression* l, Expression* r) : left(l), right(r) {}
  virtual ~BinaryOperation(){}
  Expression* getRight();
  Expression* getLeft();
  void setRight(Expression* r);
  void setLeft(Expression* l);
};

class Plus : public BinaryOperation{
 public:
  Plus(Expression* l, Expression* r) : BinaryOperation(l, r){}
  virtual ~Plus(){}
  double calculate() override;
};

class Minus : public BinaryOperation{
 public:
  Minus(Expression* l, Expression* r) : BinaryOperation(l, r){}
  virtual ~Minus(){}
  double calculate() override;
};

class Mul : public BinaryOperation{
 public:
  Mul(Expression* l, Expression* r) : BinaryOperation(l, r){}
  virtual ~Mul(){}
  double calculate() override;
};

class Div : public BinaryOperation{
 public:
  Div(Expression* l, Expression* r) : BinaryOperation(l, r){}
  virtual ~Div(){}
  double calculate() override;
};
#endif //EX1_EX1_H