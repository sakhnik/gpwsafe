//
// Entry.cc
//
//     Created: 21.01.2012
//      Author: A. Sakhnik
//

#include "Entry.hh"
#include "Memory.hh"

namespace gPWS {

void *cEntry::operator new(size_t n)
{
    return SecureAllocator<cEntry>::allocate(n);
}

void cEntry::operator delete(void *p, size_t n)
{
    cEntry *q = reinterpret_cast<cEntry *>(p);
    return SecureAllocator<cEntry>::deallocate(q, n);
}

void cEntry::AddField(sField::PtrT const &field)
{
    _other.push_back(field);
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
