//
// Created by eyal on 07/11/19.
//

#include "ex1.h"
#include "Expression.h"

class BinaryOperatior {
 protected:
  Expression* right {nullptr};
  Expression* left {nullptr};

 public:
  BinaryOperatior(Expression* right, Expression* left) : left(left), right(right) {}
  Expression* getRight() {
    return this->right;
  }
  Expression* getLeft() {
    return this->left;
  }
};

