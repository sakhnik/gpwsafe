//
// Random.hh
//
//     Created: 27.01.2012
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

#include <stdint.h>
#include <boost/noncopyable.hpp>

namespace gPWS {

class cRandom
    : boost::noncopyable
{
public:
    static void CreateNonce(uint8_t *buffer, unsigned length);

    static void CreateNonce(char *buffer, unsigned length)
    {
        CreateNonce(reinterpret_cast<uint8_t *>(buffer), length);
    }

    static void Randomize(uint8_t *buffer, unsigned length,
                          bool very_strong);

    static void Randomize(char *buffer, unsigned length,
                          bool very_strong)
    {
        Randomize(reinterpret_cast<uint8_t *>(buffer), length,
                  very_strong);
    }
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
