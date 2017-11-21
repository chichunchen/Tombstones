/////////////////////////////////////////////////////////////////////////////
// foo10.cpp, test file for CS254, assignment 5
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

int main(int argc, char **argv)
{
    int *a = new int(12);
    cout << "If *a is " << *a << endl;
    
    Pointer<int, false> tmp;
    tmp = (Pointer<int, false>) a;
    cout << "then *((Pointer<int>) a) is " << *tmp << endl;

    free(tmp);
    
    Pointer<int, false> b(new int(12));
    cout << "If *b is " << *b << endl;
    cout << "then *((int*) b) is " << *((int*) b) << endl;

    free(b);

    cout << "foo10: OK (Test for type cast)" << endl;

    return 0;
}
