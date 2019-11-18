#include <iostream>

#include "ex1.h"
#include "Expression.h"


int main() {

  // 1
  Variable *x1 = new Variable("x1", 3);// x1=3
  Expression* e1 = nullptr;
  try {
    e1 = new Mul( new UMinus(new Value(5.0) ) , new Plus( new Value(3.5) , &(++(*x1))) );// -5*(3.5+(++x1))
    cout << "1: " << e1->calculate() <<endl; //-37.5
    delete e1;
  } catch (const char* e) {
    if (e1!= nullptr) {
      delete e1;
    }
    std::cout << e << std::endl;
  }


  // 2
  Variable *x2 = new Variable("x2", 5.0);// x2=5.0
  Variable *x3 = new Variable("x3", 10.0);// x3=10.0
  Expression* e2 = nullptr;
  try {
    e2 = new Div(x3, new UMinus(new UPlus(new UMinus(x2))));// 10/-(+(-(5)))
    cout << "2: " << e2->calculate() << endl; //2
    delete e2;
  } catch (const char* e) {
    if (e2!= nullptr) {
      delete e2;
    }
    std::cout << e << std::endl;
  }

  // 3
  Variable *x4 = new Variable("x4", 2.0);// x4=2.0
  Variable *x5 = new Variable("x5", -4.5);// x5=-4.5
  Expression* e3 = nullptr;
  try {
    e3 = new Mul(&(++(*x4)), &((*x5)++));// (++x4)*(x5++)
    cout << "3: " << e3->calculate() << endl; //-10.5
    delete e3;
  } catch (const char* e) {
    if (e3!= nullptr) {
      delete e3;
    }
    std::cout << e << std::endl;
  }

  // 4
  Interpreter* i1 = new Interpreter();
  Expression* e4 = nullptr;
  try {
    e4 = i1->interpret("-(2*(3+4))");
    std::cout << "4: " << e4->calculate() << std::endl;//-14
    delete e4;
  } catch (const char* e) {
    if (e4!= nullptr) {
      delete e4;
    }
    if (i1 != nullptr) {
      delete i1;
    }
    std::cout << e << std::endl;
  }

  // 5
  Interpreter* i2 = new Interpreter();
  Expression* e5 = nullptr;
  try {
    i2->setVariables("x=2;y=4");
    i2->setVariables("x=3");
    e5 = i2->interpret("2*(-(x+y))");

    std::cout << "5: " << e5->calculate() << std::endl;//-14
    delete e5;
  } catch (const char* e) {
    if (e5!= nullptr) {
      delete e5;
    } //deleting i2 in the next example
    std::cout << e << std::endl;
  }

  // 6
  Interpreter* i3 = new Interpreter();
  Expression* e6 = nullptr;
  try {
    i3->setVariables("x=1.5;y=8.5");
    i2->setVariables("x=3");
    e6 = i3->interpret("-(-(-((x+0.5)*(y+(-3.5)))))");
    std::cout << "6: " << e6->calculate() << std::endl;//-10
    delete e6;
    delete i2;
    delete i3;
  } catch (const char* e) {
    if (e6 != nullptr) {
      delete e6;
    }
    if (i2 != nullptr) {
      delete i2;
    }
    if (i3 != nullptr) {
      delete i3;
    }
    std::cout << e << std::endl;
  }

  Interpreter* i4 = new Interpreter();
  Expression* e7 = nullptr;
  try {
    // 7
    i4->setVariables("x2=@;y=8.5");//error
    e7 = i4->interpret("-(-(-((x+0.5)*(y+(-3.5)))))");
    std::cout << "7: " << e7->calculate() << std::endl; //error
    delete e7;
    delete i4;
  } catch (const char* e) {
    if (e7 != nullptr) {
      delete e7;
    }
    if (i4 != nullptr) {
      delete i4;
    }
    std::cout << e << std::endl;
  }

  // 8
  Expression* e8 = nullptr;
  try {
    e8 = new Div(new Value(1.0), new Value(0.0));// 1.0/0.0
    std::cout << "8: " << e8->calculate() << std::endl;// error
    delete e8;
  } catch (const char* e) {
    if (e8 != nullptr) {
      delete e8;
    }
    std::cout << e << std::endl;
  }

  // 9
  Interpreter* i6 = new Interpreter();
  Expression* e9 = nullptr;
  try {
    e9 = i6->interpret("1.0-(-(-16.0))");
    std::cout << "9: " << e9->calculate() << std::endl;// -15
    delete e9;
  } catch (const char* e) {
    if (e9 != nullptr) {
      delete e9;
    }
    if (i6 != nullptr) {
      delete i6;
    }
    std::cout << e << std::endl;
  }
  // 10
  Interpreter* i7 = new Interpreter();
  Expression* e10 = nullptr;
  try {
    i7->setVariables("x=1;y=2;z=3");
    e10 = i7->interpret("-(-(-((z+0.5)*(y+(-3.5)))))");
    std::cout << "10: " << e10->calculate() << std::endl;// 5.25
    delete e10;
  } catch (const char* e) {
    if (e10 != nullptr) {
      delete e10;
    }
    if (i7 != nullptr) {
      delete i7;
    }
    std::cout << e << std::endl;
  }

  Interpreter* i8 = new Interpreter();
  Expression* e11 = nullptr;
  try {
    // 11
    i8->setVariables("x2=14;y=8.5");
    e11 = i8->interpret("-(-(-((x+0.5)**(y+(-3.5)))))"); //error
    std::cout << "11: " << e11->calculate() << std::endl;
    delete e11;
    delete i8;
  } catch (const char* e) {
    if (e11 != nullptr) {
      delete e11;
    }
    if (i8 != nullptr) {
      delete i8;
    }
    std::cout << e << std::endl;
  }

  cout << "\nExtra Tests:\n" << endl;
  //12 : var name 4x2 in setVariables
  cout << "Test 12: var name 4x2 in setVariables:" << endl;
  Interpreter* i9 = new Interpreter();
  Expression* e12 = nullptr;
  try {
    // 12
    i9->setVariables("4x2=14;y=8.5");
    e12 = i9->interpret("-(-(-((x+0.5)**(y+(-3.5)))))"); //error
    std::cout << "12: " << e12->calculate() << std::endl;
    delete e12;
    delete i9;
  } catch (const char* e) {
    if (e12 != nullptr) {
      delete e12;
    }
    if (i9 != nullptr) {
      delete i9;
    }
    std::cout << e << std::endl;
  }
  //13 : . in var name in setvariables
  cout << "\nTest 13: . in var name in setvariables:" << endl;
  Interpreter* i10 = new Interpreter();
  Expression* e13 = nullptr;
  try {
    // 13
    i10->setVariables("x.2=14;y=8.5");
    e13 = i10->interpret("-(-(-((x+0.5)**(y+(-3.5)))))"); //error
    std::cout << "13: " << e13->calculate() << std::endl;
    delete e13;
    delete i10;
  } catch (const char* e) {
    if (e13 != nullptr) {
      delete e13;
    }
    if (i10 != nullptr) {
      delete i10;
    }
    std::cout << e << std::endl;
  }
  //14 : == in var name in setvariables
  cout << "\nTest 14: == in var name in setVariables:" << endl;
  Interpreter* i11 = new Interpreter();
  Expression* e14 = nullptr;
  try {
    // 14
    i11->setVariables("x2==14;y=8.5");
    e14 = i11->interpret("-(-(-((x+0.5)**(y+(-3.5)))))"); //error
    std::cout << "14: " << e14->calculate() << std::endl;
    delete e14;
    delete i11;
  } catch (const char* e) {
    if (e14 != nullptr) {
      delete e14;
    }
    if (i11 != nullptr) {
      delete i11;
    }
    std::cout << e << std::endl;
  }

  //15 : no ; between assigments in setVariables
  cout << "\nTest 15: no ; between assigments in setVariables:" << endl;
  Interpreter* i12 = new Interpreter();
  Expression* e15 = nullptr;
  try {
    // 15
    i12->setVariables("x2=14y=8.5");
    e15 = i12->interpret("-(-(-((x+0.5)**(y+(-3.5)))))"); //error
    std::cout << "15: " << e15->calculate() << std::endl;
    delete e15;
    delete i12;
  } catch (const char* e) {
    if (e15 != nullptr) {
      delete e15;
    }
    if (i12 != nullptr) {
      delete i12;
    }
    std::cout << e << std::endl;
  }

  //16 : two ; between assigments in setVariables
  cout << "\nTest 16: two ; between assigments in setVariables:" << endl;
  Interpreter* i13 = new Interpreter();
  Expression* e16 = nullptr;
  try {
    // 16
    i13->setVariables("x2=14;;y=8.5");
    e16 = i13->interpret("-(-(-((x+0.5)**(y+(-3.5)))))"); //error
    std::cout << "16: " << e16->calculate() << std::endl;
    delete e16;
    delete i13;
  } catch (const char* e) {
    if (e16 != nullptr) {
      delete e16;
    }
    if (i13 != nullptr) {
      delete i13;
    }
    std::cout << e << std::endl;
  }
  //17 : space instead of ; between assigments in setVariables
  cout << "\nTest 17: space instead of ; between assigments in setVariables:" << endl;
  Interpreter* i14 = new Interpreter();
  Expression* e17 = nullptr;
  try {
    // 17
    i14->setVariables("x2=14 y=8.5");
    e17 = i14->interpret("-(-(-((x+0.5)**(y+(-3.5)))))"); //error
    std::cout << "17: " << e17->calculate() << std::endl;
    delete e17;
    delete i14;
  } catch (const char* e) {
    if (e17 != nullptr) {
      delete e17;
    }
    if (i14 != nullptr) {
      delete i14;
    }
    std::cout << e << std::endl;
  }
  //18 : x2=14;y=8.5, -(x2)+(-y), should work
  cout << "\nTest 18: x2=14;y=8.5, -(x)+(-y):" << endl;
  Interpreter* i15 = new Interpreter();
  Expression* e18 = nullptr;
  try {
    // 18
    i15->setVariables("x2=14;y=8.5");
    e18 = i15->interpret("-(x2)+(-y)");
    std::cout << "18: " << e18->calculate() << std::endl;
    delete e18;
    delete i15;
  } catch (const char* e) {
    if (e18 != nullptr) {
      delete e18;
    }
    if (i15 != nullptr) {
      delete i15;
    }
    std::cout << e << std::endl;
  }
  //19 : 4hello
  cout << "\nTest 19: 4hello:" << endl;
  Interpreter* i16 = new Interpreter();
  Expression* e19 = nullptr;
  try {
    // 19
    i16->setVariables("4hello");
    e19 = i16->interpret("-(x2)+(-y)");
    std::cout << "19: " << e19->calculate() << std::endl;
    delete e19;
    delete i16;
  } catch (const char* e) {
    if (e19 != nullptr) {
      delete e19;
    }
    if (i16 != nullptr) {
      delete i16;
    }
    std::cout << e << std::endl;
  }
  //20 : X4=5;y=177, should work
  cout << "\nX4=5;y=177:" << endl;
  Interpreter* i17 = new Interpreter();
  Expression* e20 = nullptr;
  try {
    // 20
    i17->setVariables("X4=5;y=177");
    e20 = i17->interpret("-(X4)+(-y)");
    std::cout << "20: " << e20->calculate() << std::endl;
    delete e20;
    delete i17;
  } catch (const char* e) {
    if (e20 != nullptr) {
      delete e20;
    }
    if (i17 != nullptr) {
      delete i17;
    }
    std::cout << e << std::endl;
  }
  //21 : X4_5=5;y=177
  cout << "\nX4_5=5;y=177:" << endl;
  Interpreter* i18 = new Interpreter();
  Expression* e21 = nullptr;
  try {
    // 21
    i18->setVariables("X4_5=5;y=177");
    e21 = i18->interpret("-(X4_5)+(-y)");
    std::cout << "21: " << e21->calculate() << std::endl; //error
    delete e20;
    delete i18;
  } catch (const char* e) {
    if (e21 != nullptr) {
      delete e21;
    }
    if (i18 != nullptr) {
      delete i18;
    }
    std::cout << e << std::endl;
  }
  //22 : x4.0=5;y=177
  cout << "\nx4.0=5;y=177:" << endl;
  Interpreter* i19 = new Interpreter();
  Expression* e22 = nullptr;
  try {
    // 22
    i19->setVariables("x4.0=5;y=177");
    e22 = i19->interpret("-(x4.0)+(-y)");
    std::cout << "22: " << e22->calculate() << std::endl; //error
    delete e20;
    delete i19;
  } catch (const char* e) {
    if (e22 != nullptr) {
      delete e22;
    }
    if (i19 != nullptr) {
      delete i19;
    }
    std::cout << e << std::endl;
  }
  //23 : x40=5;y$=177
  cout << "\nx40=5;y$=177:" << endl;
  Interpreter* i20 = new Interpreter();
  Expression* e23 = nullptr;
  try {
    // 23
    i20->setVariables("x40=5;y$=177");
    e23 = i20->interpret("-(x40)+(-y$)");
    std::cout << "23: " << e23->calculate() << std::endl; //error
    delete e23;
    delete i20;
  } catch (const char* e) {
    if (e23 != nullptr) {
      delete e23;
    }
    if (i20 != nullptr) {
      delete i20;
    }
    std::cout << e << std::endl;
  }
  //24 : x=5, -3*x
  cout << "\nx=5, -3*x:" << endl;
  Interpreter* i21 = new Interpreter();
  Expression* e24 = nullptr;
  try {
    // 24
    i21->setVariables("x=5");
    e24 = i21->interpret("-3*x");
    std::cout << "24: " << e24->calculate() << std::endl; //should work
    delete e24;
    delete i21;
  } catch (const char* e) {
    if (e24 != nullptr) {
      delete e24;
    }
    if (i21 != nullptr) {
      delete i21;
    }
    std::cout << e << std::endl;
  }
  //25 : x=5, -(+(-(x)))
  cout << "\nx=5, -(+(-(x))):" << endl;
  Interpreter* i22 = new Interpreter();
  Expression* e25 = nullptr;
  try {
    // 25
    i22->setVariables("x=5");
    e25 = i22->interpret("-(+(-(x)))");
    std::cout << "25: " << e25->calculate() << std::endl; //should work
    delete e25;
    delete i22;
  } catch (const char* e) {
    if (e25 != nullptr) {
      delete e25;
    }
    if (i22 != nullptr) {
      delete i22;
    }
    std::cout << e << std::endl;
  }
  return 0;
}

/*
 * need to add tests:
 * x4==5 //error
 * g.0=0.67 //error
 * yz=34gh //error
 * x=1.6;y=4.0 // fine
 *
 */
/*
1: -37.5
2: 2
3: -10.5
4: -14
5: -14
6: -10
illegal variable assignment!
8: division by zero
9: -15
10: 5.25
illegal math expression
*/