//
// Database.cc
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

#include "Database.hh"
#include "Memory.hh"
#include "Debug.hh"

#include <iostream>
#include <boost/format.hpp>
#include <boost/bind.hpp>

namespace gPWS {

using namespace std;

void *cDatabase::operator new(size_t n)
{
    return SecureAllocator<cDatabase>::allocate(n);
}

void cDatabase::operator delete(void *p, size_t n)
{
    cDatabase *q = reinterpret_cast<cDatabase *>(p);
    return SecureAllocator<cDatabase>::deallocate(q, n);
}

//void cDatabase::_OnField(sField::PtrT const &field,
//                         sReadCtxt &read_ctxt)
//{
//    cout << "Length: " << boost::format("%3d") % field->length;
//    cout << "\tType: "
//         << boost::format("%02X") % unsigned(field->type);
//    cout << "\tValue: "
//         << gPWS::Quote(&field->value[0], field->value.size());
//    cout << endl;
//}

void cDatabase::Read(char const *fname,
                     char const *pass)
{
    _file.OpenRead(fname, pass);

    // Read header fields first
    sField::PtrT field;
    while ((field = _file.ReadField()))
    {
        if (!_AddField(field))
            break;
    }

    if (!field)
        return;

    cEntry::PtrT entry(new cEntry);
    while ((field = _file.ReadField()))
    {
        if (!entry->AddField(field))
        {
            _entries.push_back(entry);
            entry.reset(new cEntry);
        }
    }
}

bool cDatabase::_AddField(sField::PtrT const &field)
{
    switch (field->type)
    {
    case 0x00:
        if (field->value.size() != 2)
            throw runtime_error("Format error");
        _version.minor = field->value[0];
        _version.major = field->value[1];
        break;
    case 0xFF:
        return false;
    default:
        _other.push_back(field);
        break;
    }
    return true;
}

cDatabase::EntriesT
cDatabase::Find(char const *query)
{
    EntriesT found;
    for (EntriesT::const_iterator i = _entries.begin();
         i != _entries.end(); ++i)
    {
        cEntry::PtrT entry(*i);
        if (!query || entry->GetFullTitle().find(query) != StringX::npos)
            found.push_back(entry);
    }
    return found;
}

void cDatabase::Dump() const
{
    cout << "Version: " << unsigned(_version.major)
         << '.' << unsigned(_version.minor) << endl;
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
    cout << "==========" << endl;

    for (EntriesT::const_iterator i = _entries.begin();
         i != _entries.end(); ++i)
    {
        cEntry::PtrT const& entry = *i;
        entry->Dump();
    }
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
