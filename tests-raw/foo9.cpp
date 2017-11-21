/////////////////////////////////////////////////////////////////////////////
// foo9.cpp, test file for CS254, assignment 5
/////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdlib.h>
#include "tombstones.h"

using namespace std;

void error(const char *text)
{
    cout << "ERROR: " << text << endl;
    exit(-1);
}

// Base class
class Base {
   protected:
      int radius;

   public:
      int test;

      void setRadius(int r) {
         radius = r;
      }
};

// Derived class
class Circle: public Base {
   public:
      int getArea() {
         return radius * radius * 3.14;
      }
};

int main(int argc, char **argv)
{
    Pointer<Circle, false> foo(new Circle());

    // foo can call the method in its base class
    foo->setRadius(1);

    // foo can modify the value in its base class if th value is public
    foo->test = 0;

    // foo can call its own method
    cout << "foo->getArea() - from derived class" << foo->getArea() << endl;
    
    free(foo);

    cout << "foo9: OK (Test for inheritance)" << endl;
    return 0;
}
