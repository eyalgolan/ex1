//
// Created by eyal on 07/11/19.
//

#ifndef EX1_EX1_H
#define EX1_EX1_H

#include "Expression.h"
#include "string"
class Value : public Expression{
 private:
  double number;
 public:
  explicit Value(double number) : number(number){}
  double calculate() override;
};

class Variable : public Expression {
 private:
  string name;
  double value;
 public:
  Variable(string name, double value) : name(name), value(value) {}
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
  Expression* getExp();
  void setExp(Expression* e);
};

class UPlus : public UnaryOperation {
 public:
  explicit UPlus(Expression* exp) : UnaryOperation(exp) {}
  double calculate(Expression* exp);
};

class UMinus : public UnaryOperation {
 public:
  explicit UMinus(Expression* exp) : UnaryOperation(exp) {}
  double calculate(Expression* exp);
};

class BinaryOperation : public Expression {
 protected:
  Expression* right {nullptr};
  Expression* left {nullptr};

 public:
  BinaryOperation(Expression* right, Expression* left) : left(left), right(right) {}
  Expression* getRight();
  Expression* getLeft();
  void setRight(Expression* r);
  void setLeft(Expression* l);
};

class Plus : public BinaryOperation{
 public:
  Plus(Expression* right, Expression* left) : BinaryOperation(right, left){}
  double calculate() override;
};

class Minus : public BinaryOperation{
 public:
  Minus(Expression* right, Expression* left) : BinaryOperation(right, left){}
  double calculate() override;
};

class Mul : public BinaryOperation{
 public:
  Mul(Expression* right, Expression* left) : BinaryOperation(right, left){}
  double calculate() override;
};

class Div : public BinaryOperation{
 public:
  Div(Expression* right, Expression* left) : BinaryOperation(right, left){}
  double calculate() override;
};
#endif //EX1_EX1_H
