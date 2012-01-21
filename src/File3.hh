//
// File3.hh
//
//     Created: 10.01.2012
//      Author: A. Sakhnik
//

#pragma once

#include "Field.hh"
#include "Twofish.hh"
#include "Hmac.hh"

#include <fstream>
#include <boost/noncopyable.hpp>

namespace gPWS {

class cFile3
    : boost::noncopyable
{
public:
    cFile3();
    ~cFile3();

    void Close();

    void OpenRead(char const *fname,
                  char const *pass);

    sField::PtrT ReadField();

private:
    enum _eState
    {
        S_CLOSED = 0,
        S_READING,
        S_WRITING
    } _state;

    std::fstream _fs;
    std::ios_base::iostate _initial_state;

    std::auto_ptr<cTwofish> _twofish;
    std::auto_ptr<cHmac> _hmac_calculator;
    StringX _data;
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
