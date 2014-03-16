//
// Memory.cc
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

#include "Memory.hh"
#include "i18n.h"

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
		cerr << _("WARNING: unable to use secure ram (need to have CAP_IPC_LOCK)")
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

// vim: set noet ts=4 sw=4 tw=80:
