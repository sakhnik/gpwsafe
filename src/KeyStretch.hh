//
// KeyStretch.hh
//
//     Created: 14.01.2012
//      Author: A. Sakhnik
//

#pragma once

#include "Sha256.hh"

namespace gPWS {

class cKeyStretch
{
public:
    cKeyStretch(void const* pass,
                unsigned pass_size,
                void const* salt,
                unsigned salt_size,
                unsigned iterations);

    ~cKeyStretch();

    static const unsigned LENGTH = cSha256::LENGTH;

    uint8_t const *Get() const;

private:
    cSha256 _sha256;

    cKeyStretch(cKeyStretch const &);
    cKeyStretch &operator=(cKeyStretch const &);
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
