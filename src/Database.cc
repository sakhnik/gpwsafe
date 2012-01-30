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
#include <uuid/uuid.h>

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

cDatabase::cDatabase()
{
    _fields.reserve(0x0F);
}

void cDatabase::Create()
{
    _fields.clear();
    sField::PtrT field(new sField);

    // Insert default version (mock 3.10)
    field->type = FT_VERSION;
    field->value.clear();
    field->value.push_back(0x0A);
    field->value.push_back(0x03);
    _AddField(field);

    // Insert new UUID
    field->type = FT_UUID;
    field->value.resize(16);
    uuid_t new_uuid;
    ::uuid_generate(new_uuid);
    copy(new_uuid, new_uuid + 16, field->value.begin());
    _AddField(field);
}

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

void cDatabase::Write(char const *fname,
                      char const *pass)
{
    _file.OpenWrite(fname, pass, true);

    for (_FieldsT::const_iterator i = _fields.begin();
         i != _fields.end(); ++i)
    {
        sField::PtrT const &field = *i;
        if (field)
            _file.WriteField(field);
    }

    // Field terminator
    sField::PtrT terminator(new sField);
    terminator->type = 0xFF;
    terminator->value.clear();
    _file.WriteField(terminator);

    for (EntriesT::const_iterator i = _entries.begin();
         i != _entries.end(); ++i)
    {
        cEntry::PtrT const &entry(*i);
        entry->ForEachField(boost::bind(&cFile3::WriteField,
                                        boost::ref(_file),
                                        _1));

        _file.WriteField(terminator);
    }

    _file.CloseWrite();
}

bool cDatabase::_AddField(sField::PtrT const &field)
{
    if (field->type == 0xFF)
        return false;
    if (_fields.size() <= field->type)
        _fields.resize(static_cast<unsigned>(field->type) + 1);
    _fields[field->type] = field;
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
