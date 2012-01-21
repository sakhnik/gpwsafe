//
// Entry.cc
//
//     Created: 21.01.2012
//      Author: A. Sakhnik
//

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

} //namespace gPWS;

// vim: set et ts=4 sw=4:
