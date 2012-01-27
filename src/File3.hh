//
// File3.hh
//
//     Created: 10.01.2012
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

    void OpenWrite(char const *fname,
                   char const *pass);

private:
    enum _eState
    {
        S_CLOSED = 0,
        S_READING,
        S_WRITING
    } _state;

    std::fstream _fs;
    std::ios_base::iostate _initial_state;

    // Count of iterations for the key stretching
    uint32_t _iterations;

    std::auto_ptr<cTwofish> _twofish;
    std::auto_ptr<cHmac> _hmac_calculator;
    StringX _data;
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
