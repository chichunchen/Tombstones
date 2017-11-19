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
    Pointer<Circle, true> foo(new Circle());
    foo->setRadius(1);
    foo->test = 0;
    cout << foo->getArea() << endl;
    
    free(foo);

    cout << "foo9: OK" << endl;
    return 0;
}
