//
// Debug.hh
//
//     Created: 14.01.2012
//      Author: A. Sakhnik
//

#pragma once

#include <string>

namespace gPWS {

void PrintBuf(unsigned char const *buf, unsigned len);

void PrintBuf(char const *buf, unsigned len);

std::string Quote(unsigned char const *buf, unsigned len);

std::string Quote(char const *buf, unsigned len);

} //namespace gPWS;

// vim: set et ts=4 sw=4:
