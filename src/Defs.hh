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

typedef std::vector<uint8_t> BytesT;

typedef std::vector<uint8_t, SecureAllocator<uint8_t> > SecureBytesT;

typedef std::basic_string<char,
                          std::char_traits<char>,
                          SecureAllocator<char> > SecureStringT;

} //namespace gPWS;

// vim: set et ts=4 sw=4:
