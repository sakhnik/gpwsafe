//
// Hmac.cc
//
//     Created: 17.01.2012
//      Author: A. Sakhnik
//

#include "Hmac.hh"

#include <iostream>
#include <stdexcept>

namespace gPWS {

using namespace std;

cHmac::cHmac(void const *key, size_t key_len)
{
    gcry_error_t error =
        gcry_md_open(&_h,
                     GCRY_MD_SHA256,
                     GCRY_MD_FLAG_SECURE | GCRY_MD_FLAG_HMAC);
    if (error)
    {
        cerr << "gcry_md_open failed: "
             << gcry_strsource(error) << "/"
             << gcry_strerror(error) << endl;
        throw runtime_error("md error");
    }

    error = gcry_md_setkey(_h, key, key_len);
    if (error)
    {
        cerr << "gcry_md_setkey failed: "
             << gcry_strsource(error) << "/"
             << gcry_strerror(error) << endl;
        throw runtime_error("md error");
    }
}

cHmac::~cHmac()
{
    gcry_md_close(_h);
}

void cHmac::Update(void const *data, size_t len)
{
    gcry_md_write(_h, data, len);
}

uint8_t const *cHmac::Get() const
{
    return gcry_md_read(_h, 0);
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
