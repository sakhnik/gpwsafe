//
// Twofish.hh
//
//     Created: 17.01.2012
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

#include <gcrypt.h>
#include <stdint.h>
#include <boost/noncopyable.hpp>
#include <boost/static_assert.hpp>

namespace gPWS {

class cTwofish
    : boost::noncopyable
{
public:
    enum eMode
    {
        M_ECB = GCRY_CIPHER_MODE_ECB,
        M_CBC = GCRY_CIPHER_MODE_CBC
    };

    static const unsigned KEY_LENGTH = 32;
    static const unsigned BLOCK_LENGTH = 16;

    cTwofish(eMode mode,
             void const *key, size_t key_len);

    ~cTwofish();

    void SetIV(void const *iv, size_t iv_len);

    void Decrypt(uint8_t *out, size_t out_len,
                 uint8_t const *in, size_t in_len);

    template<typename A, typename B>
    void Decrypt(A *out, size_t out_len,
                 B const *in, size_t in_len)
    {
        BOOST_STATIC_ASSERT(sizeof(A) == sizeof(uint8_t));
        BOOST_STATIC_ASSERT(sizeof(B) == sizeof(uint8_t));

        Decrypt(reinterpret_cast<uint8_t *>(out), out_len,
                reinterpret_cast<uint8_t const *>(in), in_len);
    }

    void Encrypt(uint8_t *out, size_t out_len,
                 uint8_t const *in, size_t in_len);

    template<typename A, typename B>
    void Encrypt(A *out, size_t out_len,
                 B const *in, size_t in_len)
    {
        BOOST_STATIC_ASSERT(sizeof(A) == sizeof(uint8_t));
        BOOST_STATIC_ASSERT(sizeof(B) == sizeof(uint8_t));

        Encrypt(reinterpret_cast<uint8_t *>(out), out_len,
                reinterpret_cast<uint8_t const *>(in), in_len);
    }

private:
    gcry_cipher_hd_t _h;
};

} //namespace gPWS;

// vim: set et ts=4 sw=4 tw=80:
