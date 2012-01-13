//
// File3.hh
//
//     Created: 10.01.2012
//      Author: A. Sakhnik
//

#pragma once

#include <string>
#include <fstream>

namespace gPWS {

class cFile3
{
public:
    cFile3();
    ~cFile3();

    int Open(char const* fname);
    int Close();

private:
    std::string _fname;
    std::fstream _file;
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
