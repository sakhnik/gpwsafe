//
// Entry.cc
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

#include "Entry.hh"
#include "Memory.hh"
#include "Debug.hh"

#include <iostream>
#include <boost/format.hpp>

namespace gPWS {

using namespace std;

void *cEntry::operator new(size_t n)
{
    return SecureAllocator<cEntry>::allocate(n);
}

void cEntry::operator delete(void *p, size_t n)
{
    cEntry *q = reinterpret_cast<cEntry *>(p);
    return SecureAllocator<cEntry>::deallocate(q, n);
}

cEntry::cEntry()
    : _fields(0x0F, sField::PtrT())
{
}

bool cEntry::AddField(sField::PtrT const &field)
{
    if (field->type == sField::T_END)
        return false;
    if (_fields.size() <= field->type)
        _fields.resize(field->type + 1, sField::PtrT());
    _fields[field->type] = field;
    return true;
}

void cEntry::ForEachField(OnFieldT on_field)
{
    for (_FieldsT::const_iterator i = _fields.begin(); i != _fields.end(); ++i)
    {
        sField::PtrT const &field(*i);
        if (!field)
            continue;
        on_field(field);
    }
}

void cEntry::Dump() const
{
    for (_FieldsT::const_iterator i = _fields.begin();
         i != _fields.end(); ++i)
    {
        sField::PtrT const& field = *i;
        if (!field)
            continue;
        cout << "{0x"
             << boost::format("%02X") % unsigned(field->type)
             << ", '"
             << gPWS::Quote(&field->value[0], field->value.size())
             << "'}"
             << endl;
    }
    cout << "----------" << endl;
}

StringX const &cEntry::GetValue(sField::eType type) const
{
    if (type >= _fields.size())
        return _empty;
    sField::PtrT const &field = _fields[type];
    if (!field)
        return _empty;
    return field->value;
}

StringX cEntry::GetFullTitle() const
{
    if (GetGroup().empty())
        return GetTitle();
    return GetGroup() + "." + GetTitle();
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
