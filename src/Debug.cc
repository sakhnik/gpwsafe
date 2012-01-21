//
// Debug.cc
//
//     Created: 14.01.2012
//      Author: A. Sakhnik
//

#include "Debug.hh"

#include <iostream>
#include <cstdio>
#include <boost/format.hpp>

namespace gPWS {

using namespace std;

void PrintBuf(unsigned char const *buf, unsigned len)
{
    for (unsigned i = 0; i != len; ++i)
        cout << boost::format("%02X") % buf[i];
    cout << endl;
}

void PrintBuf(char const *buf, unsigned len)
{
    PrintBuf(reinterpret_cast<unsigned char const *>(buf), len);
}

string Quote(unsigned char const *buf, unsigned len)
{
    string result;
    for (unsigned i = 0; i != len; ++i)
    {
        unsigned char c = buf[i];
        if (isprint(c))
        {
            result.push_back(c);
            continue;
        }
        if (c == '\\')
        {
            result.push_back('\\');
            result.push_back('\\');
            continue;
        }
        boost::format fmt("\\x%02X");
        result += (fmt % static_cast<unsigned>(c)).str();
    }
    return result;
}

string Quote(char const *buf, unsigned len)
{
    return Quote(reinterpret_cast<unsigned char const *>(buf), len);
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
