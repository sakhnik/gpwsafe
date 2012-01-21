//
// Field.cc
//
//     Created: 21.01.2012
//      Author: A. Sakhnik
//

#include "Field.hh"
#include "Memory.hh"

namespace gPWS {

void *sField::operator new(size_t n)
{
    return SecureAllocator<sField>::allocate(n);
}

void sField::operator delete(void *p, size_t n)
{
    sField *q(reinterpret_cast<sField *>(p));
    return SecureAllocator<sField>::deallocate(q, n);
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
