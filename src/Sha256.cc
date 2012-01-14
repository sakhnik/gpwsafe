//
// Sha256.cc
//
//     Created: 14.01.2012
//      Author: A. Sakhnik
//

#include "Sha256.hh"

namespace gPWS {

cSha256::cSha256()
{
    gcry_md_open(&_c, GCRY_MD_SHA256, GCRY_MD_FLAG_SECURE);
}

cSha256::cSha256(void const *data, unsigned len)
{
    gcry_md_open(&_c, GCRY_MD_SHA256, GCRY_MD_FLAG_SECURE);
    gcry_md_write(_c, data, len);
}

cSha256::~cSha256()
{
    gcry_md_close(_c);
}

uint8_t const *cSha256::Get() const
{
    return gcry_md_read(_c, 0);
}

void cSha256::Update(void const *data, unsigned len)
{
    gcry_md_write(_c, data, len);
}

void cSha256::Reset()
{
    gcry_md_reset(_c);
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
