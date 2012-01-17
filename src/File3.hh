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

    int Open(char const *fname,
             char const *pass);

    int Close();

private:
    std::fstream _file;

    cFile3(cFile3 const &);
    cFile3& operator=(cFile3 const &);
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
