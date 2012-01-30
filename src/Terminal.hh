//
// Terminal.hh
//
//     Created: 22.01.2012
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

#include "Defs.hh"

#include <termios.h>
#include <boost/noncopyable.hpp>

namespace gPWS {

class cTerminal
    : boost::noncopyable
{
public:
    static StringX GetPassword(char const *prompt);
};

class cRawTerminal
    : public cTerminal
{
public:
    cRawTerminal();
    ~cRawTerminal();

private:
    struct termios _tio;
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
