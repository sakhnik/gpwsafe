//
// KeyStretch.hh
//
//     Created: 14.01.2012
//      Author: A. Sakhnik
//
// This file is part of gpwsafe.
//
// Gpwsafe is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Gpwsafe is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with gpwsafe.  If not, see <http://www.gnu.org/licenses/>

#pragma once

#include "Sha256.hh"

#include <boost/noncopyable.hpp>

namespace gPWS {

class cKeyStretch
    : boost::noncopyable
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
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
