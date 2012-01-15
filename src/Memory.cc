//
// Memory.cc
//
//     Created: 15.01.2012
//      Author: A. Sakhnik
//

#include "Memory.hh"

#include <sys/mman.h>
#include <iostream>

namespace gPWS {

using namespace std;

char *cLockedBlockAllocator::malloc(const size_type bytes)
{
    char *block = reinterpret_cast<char *>(std::malloc(bytes));
    int ret = mlock(block, bytes);
    if (ret)
    {
        // TODO: Implement setuid if needed.
        cerr << "WARNING: unable to use secure ram (need to be setuid root)"
             << endl;
    }
    return block;
}

void cLockedBlockAllocator::free(char *const block)
{
    // We can't do munlock(), because the pool doesn't let us know the size
    // of the block. Let's not bother, because any way the memory locks
    // will be removed when the process exits.
    std::free(block);
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
