//
// Memory.hh
//
//     Created: 15.01.2012
//      Author: A. Sakhnik
//

#pragma once

#include <boost/pool/pool_alloc.hpp>

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
