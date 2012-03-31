//
// Memory.hh
//
//     Created: 15.01.2012
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

#include <boost/pool/pool_alloc.hpp>
#include <stdio.h>

namespace gPWS {

class cLockedBlockAllocator
{
public:
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    static char *malloc(const size_type bytes);
    static void free(char *const block);
};

template<typename T>
class SecureAllocator
    : public boost::pool_allocator<T, cLockedBlockAllocator>
{
public:
    typedef boost::pool_allocator<T, cLockedBlockAllocator> BaseT;

    typedef T value_type;
    typedef value_type * pointer;
    typedef typename boost::pool<cLockedBlockAllocator>::size_type size_type;

    template <typename U>
    struct rebind
    {
        typedef SecureAllocator<U> other;
    };

    static void deallocate(const pointer ptr, const size_type n)
    {
        // Clear memory before returning it to the system.
        memset(ptr, 0, n*sizeof(T));
        BaseT::deallocate (ptr, n);
    }
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
