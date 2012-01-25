//
// Database.hh
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

#include "File3.hh"
#include "Entry.hh"

#include <stdint.h>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace gPWS {

class cDatabase
    : boost::noncopyable
{
public:
    typedef boost::shared_ptr<cDatabase> PtrT;

    void *operator new(size_t n);
    void operator delete(void *p, size_t n);

    void Read(char const *fname,
              char const *pass);

    void Dump() const;

    typedef std::vector<cEntry::PtrT> EntriesT;
    EntriesT Find(char const *query);

private:
    cFile3 _file;

    struct _sVersion
    {
        uint8_t major;
        uint8_t minor;
    } _version;

    typedef std::vector<sField::PtrT> _OtherT;
    _OtherT _other;

    EntriesT _entries;

    bool _AddField(sField::PtrT const &field);
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
