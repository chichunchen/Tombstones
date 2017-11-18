/////////////////////////////////////////////////////////////////////////////
// foo7.cpp, test file for CS254, assignment 5
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

void sub(Pointer<int, false> &foo)
{
    Pointer<int, false> bar(foo);
    Pointer<int, false> bat(bar);
    Pointer<int, false> qix(bat);
    Pointer<int, false> glorch(qix);

    *glorch = 100;
    if (*foo != 100)
	error("Linking of pointers not correct!");

    free(glorch);
}

int main(int argc, char **argv)
{
    Pointer<int, false> foo(new int(0));
    sub(foo);
    *foo = 1000;
    error("Didn't complain about use of dangling pointer foo!");

    return 0;
}
