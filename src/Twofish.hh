//
// Twofish.hh
//
//     Created: 17.01.2012
//      Author: A. Sakhnik
//

#pragma once

#include <gcrypt.h>
#include <stdint.h>

namespace gPWS {

class cTwofish
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

private:
    gcry_cipher_hd_t _h;

    cTwofish(cTwofish const &);
    cTwofish &operator=(cTwofish const &);
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
