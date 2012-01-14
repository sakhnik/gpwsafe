//
// KeyStretch.cc
//
//     Created: 14.01.2012
//      Author: A. Sakhnik
//

#include "KeyStretch.hh"

namespace gPWS {

using namespace std;

cKeyStretch::cKeyStretch(void const* pass,
                         unsigned pass_size,
                         void const* salt,
                         unsigned salt_size,
                         unsigned iterations)
{
    _sha256.Update(pass, pass_size);
    _sha256.Update(salt, salt_size);

    for (unsigned i = 0; i != iterations; ++i)
    {
        uint8_t X[cSha256::LENGTH];
        memcpy(X, _sha256.Get(), cSha256::LENGTH);
        _sha256.Reset();
        _sha256.Update(X, cSha256::LENGTH);
    }
}

cKeyStretch::~cKeyStretch()
{
}

uint8_t const* cKeyStretch::Get() const
{
    return _sha256.Get();
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
