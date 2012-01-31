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

class cRawTerminal
    : boost::noncopyable
{
public:
    cRawTerminal(bool new_line = false);
    ~cRawTerminal();

private:
    bool _new_line;
    struct termios _tio;
};

class cTerminal
    : boost::noncopyable
{
public:
    static StringX GetPassword(char const *prompt);
    static StringX GetText(char const *prompt);
    static StringX EnterPassword(char const *prompt1, char const *prompt2);
    static bool GetYN(char const *prompt, const int def_val);
    static char GetChar(char const *prompt, const int def_val);

    template<typename StringT>
    static char GetChar(StringT const &prompt, const int def_val)
    {
        return GetChar(prompt.c_str(), def_val);
    }

    static StringX RandomPassword();
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
