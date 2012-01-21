//
// Defs.hh
//
//     Created: 14.01.2012
//      Author: A. Sakhnik
//

#pragma once

#include "Memory.hh"

#include <stdint.h>
#include <vector>

namespace gPWS {

typedef std::vector<uint8_t> BytesT;

typedef std::basic_string<char,
                          std::char_traits<char>,
                          SecureAllocator<char> > StringX;

} //namespace gPWS;

// vim: set et ts=4 sw=4:
