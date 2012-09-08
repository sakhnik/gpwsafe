//
// Debug.cc
//
//     Created: 14.01.2012
//      Author: A. Sakhnik
//
// This file is part of gpwsafe.
//
// Gpwsafe is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Gpwsafe is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with gpwsafe.  If not, see <http://www.gnu.org/licenses/>

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

// vim: set et ts=4 sw=4 tw=80:
