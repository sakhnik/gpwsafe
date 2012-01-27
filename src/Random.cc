//
// Random.cc
//
//     Created: 27.01.2012
//      Author: A. Sakhnik
//

#include "Random.hh"

#include <gcrypt.h>

namespace gPWS {

void cRandom::CreateNonce(uint8_t *buffer, unsigned len)
{
    gcry_create_nonce(buffer, len);
}

void cRandom::Randomize(uint8_t *buffer, unsigned length)
{
    // TODO: Utilize GCRY_VERY_STRONG_RANDOM
    gcry_randomize(buffer, length, GCRY_STRONG_RANDOM);
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
