//
// Twofish.hh
//
//     Created: 17.01.2012
//      Author: A. Sakhnik
//

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

private:
    gcry_cipher_hd_t _h;
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
