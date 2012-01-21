//
// Database.cc
//
//     Created: 21.01.2012
//      Author: A. Sakhnik
//

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

struct cDatabase::sReadCtxt
{
    bool is_header;
    cEntry::PtrT entry;

    sReadCtxt() : is_header(true) { }
};

void cDatabase::_OnField(sField::PtrT const &field,
                         sReadCtxt &read_ctxt)
{
    if (read_ctxt.is_header)
    {
        switch (field->type)
        {
        case 0x00:
            if (field->value.size() != 2)
                throw runtime_error("Format error");
            _version.minor = field->value[0];
            _version.major = field->value[1];
            return;
        case 0x01:
            _uuid = field->value;
            return;
        case 0xFF:
            read_ctxt.is_header = false;
            return;
        default:
            _other.push_back(field);
            return;
        }
        return;
    }

    if (!read_ctxt.entry)
        read_ctxt.entry.reset(new cEntry);

    if (field->type == 0xFF)
    {
        _entries.push_back(read_ctxt.entry);
        read_ctxt.entry.reset(new cEntry);
        return;
    }
    read_ctxt.entry->AddField(field);

    //cout << "Length: " << boost::format("%3d") % field->length;
    //cout << "\tType: "
    //     << boost::format("%02X") % unsigned(field->type);
    //cout << "\tValue: "
    //     << gPWS::Quote(&field->value[0], field->value.size());
    //cout << endl;
}

void cDatabase::Read(char const *fname,
                     char const *pass)
{
    sReadCtxt read_ctxt;
    _file.Read(fname,
               pass,
               boost::bind(&cDatabase::_OnField, this,
                           _1, boost::ref(read_ctxt)));
}

void cDatabase::Dump() const
{
    cout << "Version: " << unsigned(_version.major)
         << '.' << unsigned(_version.minor) << endl;
    cout << "UUID: " << Quote(&_uuid[0], _uuid.size()) << endl;

    for (_OtherT::const_iterator i = _other.begin();
         i != _other.end(); ++i)
    {
        sField::PtrT const& field (*i);
        cout << "Length: " << boost::format("%3d") % field->length;
        cout << "\tType: "
             << boost::format("%02X") % unsigned(field->type);
        cout << "\tValue: "
             << gPWS::Quote(&field->value[0], field->value.size());
        cout << endl;
    }
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
