//
// Sha256.hh
//
//     Created: 14.01.2012
//      Author: A. Sakhnik
//

#pragma once

#include <gcrypt.h>
#include <stdint.h>
#include <boost/noncopyable.hpp>

namespace gPWS {

class cSha256
    : boost::noncopyable
{
public:
    static const unsigned LENGTH = 32;

    cSha256();
    cSha256(void const *data, size_t len);

    ~cSha256();

    void Update(void const *data, size_t len);
    uint8_t const *Get() const;
    void Reset();

private:
    gcry_md_hd_t _h;
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
