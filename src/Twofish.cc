//
// Twofish.cc
//
//     Created: 17.01.2012
//      Author: A. Sakhnik
//

#include "Twofish.hh"

#include <iostream>
#include <stdexcept>

namespace gPWS {

using namespace std;

cTwofish::cTwofish(eMode mode,
                   void const *key, size_t key_len)
{
    gcry_error_t error = gcry_cipher_open(&_h,
                                          GCRY_CIPHER_TWOFISH,
                                          mode,
                                          // FIXME!!!
                                          0);//GCRY_CIPHER_SECURE);
    if (error)
    {
        cerr << "gcry_cipher_open failed: "
             << gcry_strsource(error) << "/"
             << gcry_strerror(error) << endl;
        throw runtime_error("cipher error");
    }

    error = gcry_cipher_setkey(_h, key, key_len);
    if (error)
    {
        cerr << "gcry_cipher_setkey failed: "
             << gcry_strsource(error) << "/"
             << gcry_strerror(error) << endl;
        throw runtime_error("cipher error");
    }
}

cTwofish::~cTwofish()
{
    gcry_cipher_close(_h);
}

void cTwofish::SetIV(void const *iv, size_t iv_len)
{
    gcry_error_t error =
        gcry_cipher_setiv(_h, iv, iv_len);
    if (error)
    {
        cerr << "gcry_cipher_setiv failed: "
             << gcry_strsource(error) << "/"
             << gcry_strerror(error) << endl;
        throw runtime_error("cipher error");
    }
}

void cTwofish::Decrypt(uint8_t *out, size_t out_len,
                       uint8_t const *in, size_t in_len)
{
    gcry_error_t error =
        gcry_cipher_decrypt(_h, out, out_len, in, in_len);
    if (error)
    {
        cerr << "gcry_cipher_decrypt failed: "
             << gcry_strsource(error) << "/"
             << gcry_strerror(error) << endl;
        throw runtime_error("cipher error");
    }
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
