//
// Entry.hh
//
//     Created: 21.01.2012
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

#include "Field.hh"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

namespace gPWS {

class cEntry
    : boost::noncopyable
{
public:
    typedef boost::shared_ptr<cEntry> PtrT;

    void *operator new(size_t n);
    void operator delete(void *p, size_t n);

    cEntry();

    bool AddField(sField::PtrT const &field);

    typedef boost::function<void(sField::PtrT const &ptr)> OnFieldT;

    void ForEachField(OnFieldT on_field);

    void Dump() const;

    StringX const &GetValue(sField::eType type) const;

    StringX const &GetGroup() const { return GetValue(sField::T_GROUP); }
    StringX const &GetTitle() const { return GetValue(sField::T_TITLE); }
    StringX const &GetUser() const  { return GetValue(sField::T_USER);  }
    StringX const &GetNotes() const { return GetValue(sField::T_NOTES); }
    StringX const &GetPass() const  { return GetValue(sField::T_PASS);  }
    StringX const &GetUrl() const   { return GetValue(sField::T_URL);   }

    // group.title
    StringX GetFullTitle() const;

private:
    // Associative container. Type is used as index in the vector.
    // Null means the field wasn't defined.
    typedef std::vector<sField::PtrT> _FieldsT;
    _FieldsT _fields;

    // Empty string to refer to if a specific field is missing.
    StringX _empty;
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
