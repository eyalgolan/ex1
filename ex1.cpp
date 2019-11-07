//
// Created by eyal on 07/11/19.
//

#include "ex1.h"
#include "Expression.h"

class Value : public Expression {
 private:
  double number;
 public:
  Value(double number) : number(number){}
  virtual double calculate() {
    return this->number;
  }
};

class BinaryOperation : public Expression {
 protected:
  Expression* right {nullptr};
  Expression* left {nullptr};

 public:
  BinaryOperation(Expression* right, Expression* left) : left(left), right(right) {}
  virtual double calculate() = 0;
  Expression* getRight() {
    return this->right;
  }
  Expression* getLeft() {
    return this->left;
  }
  void setRight(Expression* r) {
    this->right = r;
  }
  void setLeft(Expression* l) {
    this->left = l;
  }
};

class Plus : public BinaryOperation{
 public:
  Plus(Expression* right, Expression* left) : BinaryOperation(right, left){}
  /*
  virtual double calculate() {
    return this->getLeft() + this->getRight();
  }
   */
};

