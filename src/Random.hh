//
// Random.hh
//
//     Created: 27.01.2012
//      Author: A. Sakhnik
//

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

    static void Randomize(uint8_t *buffer, unsigned length);

    static void Randomize(char *buffer, unsigned length)
    {
        Randomize(reinterpret_cast<uint8_t *>(buffer), length);
    }
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
