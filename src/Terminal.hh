//
// Terminal.hh
//
//     Created: 22.01.2012
//      Author: A. Sakhnik
//

#pragma once

#include "Defs.hh"

namespace gPWS {

class cTerminal
{
public:
    static StringX GetPassword(char const *prompt);
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
