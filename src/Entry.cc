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

bool cEntry::AddField(sField::PtrT const &field)
{
    switch (field->type)
    {
    case 0x02:
        _group = field->value;
        break;
    case 0x03:
        _title = field->value;
        break;
    case 0x04:
        _user = field->value;
        break;
    case 0x05:
        _notes = field->value;
        break;
    case 0x06:
        _pass = field->value;
        break;
    case 0x0D:
        _url = field->value;
        break;
    case 0xFF:
        return false;
    default:
        _other.push_back(field);
        break;
    }
    return true;
}

void cEntry::ForEachField(OnFieldT on_field)
{
    sField::PtrT field(new sField);
    // Group
    if (!_group.empty())
    {
        field->type = 0x02;
        field->value = _group;
        on_field(field);
    }
    // Title
    if (!_title.empty())
    {
        field->type = 0x03;
        field->value = _title;
        on_field(field);
    }
    // User
    if (!_user.empty())
    {
        field->type = 0x04;
        field->value = _user;
        on_field(field);
    }
    // Notes
    if (!_notes.empty())
    {
        field->type = 0x05;
        field->value = _notes;
        on_field(field);
    }
    // Password
    if (!_pass.empty())
    {
        field->type = 0x06;
        field->value = _pass;
        on_field(field);
    }
    // URL
    if (!_url.empty())
    {
        field->type = 0x0D;
        field->value = _url;
        on_field(field);
    }

    for (_OtherT::const_iterator i = _other.begin(); i != _other.end(); ++i)
    {
        on_field(*i);
    }
}

void cEntry::Dump() const
{
    cout << "Group: " << _group << endl;
    cout << "Title: " << _title << endl;
    cout << "User: " << _user << endl;
    cout << "Password: " << _pass << endl;
    cout << "Notes: " << _notes << endl;
    cout << "URL: " << _url << endl;
    cout << endl;

    for (_OtherT::const_iterator i = _other.begin();
         i != _other.end(); ++i)
    {
        sField::PtrT const& field = *i;
        cout << "{0x"
             << boost::format("%02X") % unsigned(field->type)
             << ", '"
             << gPWS::Quote(&field->value[0], field->value.size())
             << "'}"
             << endl;
    }
    cout << "----------" << endl;
}

StringX cEntry::GetFullTitle() const
{
    if (_group.empty())
        return _title;
    return _group + "." + _title;
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
