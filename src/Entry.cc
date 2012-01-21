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
    for (_OtherT::const_iterator i = _other.begin();
         i != _other.end(); ++i)
    {
        sField::PtrT const& field = *i;
        cout << "Length: " << boost::format("%3d") % field->length;
        cout << "\tType: "
             << boost::format("%02X") % unsigned(field->type);
        cout << "\tValue: "
             << gPWS::Quote(&field->value[0], field->value.size());
        cout << endl;
    }
    cout << "----------" << endl;
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
