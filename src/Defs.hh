//
// Defs.hh
//
//     Created: 14.01.2012
//      Author: A. Sakhnik
//

#pragma once

#include "Memory.hh"

#include <vector>
#include <stdint.h>

namespace gPWS {

typedef std::vector<char> BytesT;

typedef std::vector<char, SecureAllocator<char> > SecureBytesT;

typedef std::basic_string<char,
                          std::char_traits<char>,
                          SecureAllocator<char> > SecureStringT;

} //namespace gPWS;

// vim: set et ts=4 sw=4:
