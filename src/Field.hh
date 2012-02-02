//
// Field.hh
//
//     Created: 20.01.2012
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

#include "Defs.hh"

#include <stdint.h>
#include <iosfwd>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>

namespace gPWS {

struct sField
    : boost::noncopyable
{
    typedef boost::shared_ptr<sField> PtrT;

    uint8_t type;
    StringX value;

    sField::PtrT Copy() const;

    void *operator new(size_t n);
    void operator delete(void *p, size_t n);

    bool operator!=(sField const &o)
    {
        return type != o.type || value != o.value;
    }
};

std::ostream &operator<<(std::ostream &os, sField const &field);

} //namespace gPWS;

// vim: set et ts=4 sw=4:
