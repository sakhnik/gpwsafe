//
// Defs.hh
//
//     Created: 14.01.2012
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
