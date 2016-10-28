//
// Gcrypt.cc
//
//     Created: 29.01.2012
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


#include "Gcrypt.hh"
#include "i18n.h"

#include <iostream>
#include <gcrypt.h>
#include <boost/format.hpp>

namespace gPWS {

using namespace std;
typedef boost::format bfmt;

namespace {

void
_GcryProgressHandler(void *cb_data, const char *what,
                     int printchar, int current, int total)
{
	if (!strcmp(what, "need_entropy"))
	{
		cerr << '\r' << bfmt(_("Entropy pool: %d of %d bytes available."))
			% current % total;
		if (current == total)
			cerr << _(" Done.              ") << endl;
		else
			cerr << _(" Waiting for more...") << flush;
	}
}

} //namespace;

int Gcrypt::CheckVersion()
{
	char const *const REQ_GCRYPT_VERSION = "1.2.0";
	return 0 == gcry_check_version(REQ_GCRYPT_VERSION);
}

int Gcrypt::Init(bool quick_random)
{
	// Allocate secure memory for sensitive information (won't be swapped)
	gcry_control(GCRYCTL_INIT_SECMEM, 16384, 0);
	gcry_set_progress_handler(_GcryProgressHandler, NULL);
	if (quick_random)
		gcry_control(GCRYCTL_ENABLE_QUICK_RANDOM, 0, 0);
	gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);

	return 0;
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
