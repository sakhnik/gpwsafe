//
// Hmac.hh
//
//     Created: 17.01.2012
//      Author: A. Sakhnik
//

#pragma once

#include <gcrypt.h>
#include <stdint.h>
#include <boost/noncopyable.hpp>

namespace gPWS {

class cHmac
    : boost::noncopyable
{
public:
    cHmac(void const *key, size_t key_len);

    ~cHmac();

    void Update(void const *data, size_t len);
    uint8_t const *Get() const;

private:
    gcry_md_hd_t _h;
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
