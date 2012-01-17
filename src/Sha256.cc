//
// Sha256.cc
//
//     Created: 14.01.2012
//      Author: A. Sakhnik
//

#include "Sha256.hh"

#include <iostream>
#include <stdexcept>

namespace gPWS {

using namespace std;

cSha256::cSha256()
{
    gcry_error_t error =
        gcry_md_open(&_h, GCRY_MD_SHA256, GCRY_MD_FLAG_SECURE);
    if (error)
    {
        cerr << "gcry_md_open failed: "
             << gcry_strsource(error) << "/"
             << gcry_strerror(error) << endl;
        throw runtime_error("md error");
    }
}

cSha256::cSha256(void const *data, unsigned len)
{
    gcry_error_t error =
        gcry_md_open(&_h, GCRY_MD_SHA256, GCRY_MD_FLAG_SECURE);
    if (error)
    {
        cerr << "gcry_md_open failed: "
             << gcry_strsource(error) << "/"
             << gcry_strerror(error) << endl;
        throw runtime_error("md error");
    }

    gcry_md_write(_h, data, len);
}

cSha256::~cSha256()
{
    gcry_md_close(_h);
}

uint8_t const *cSha256::Get() const
{
    return gcry_md_read(_h, 0);
}

void cSha256::Update(void const *data, unsigned len)
{
    gcry_md_write(_h, data, len);
}

void cSha256::Reset()
{
    gcry_md_reset(_h);
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
