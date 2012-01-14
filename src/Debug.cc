//
// Debug.cc
//
//     Created: 14.01.2012
//      Author: A. Sakhnik
//

#include "Debug.hh"

#include <iostream>
#include <cstdio>

namespace gPWS {

using namespace std;

void PrintBuf(unsigned char const *buf, unsigned len)
{
    for (unsigned i = 0; i != len; ++i)
    {
        char s[24];
        sprintf(s, "%02X", buf[i]);
        cout << s;
    }
    cout << endl;
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
