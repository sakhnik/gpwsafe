//
// Gcrypt.cc
//
//     Created: 29.01.2012
//      Author: A. Sakhnik
//

#include "Gcrypt.hh"

#include <iostream>
#include <gcrypt.h>

namespace gPWS {

using namespace std;

namespace {

void
_GcryProgressHandler(void *cb_data, const char *what,
                     int printchar, int current, int total)
{
	if (!strcmp(what, "need_entropy"))
	{
		cerr << "\rEntropy pool: " << current << " of " << total
			<< " bytes are available.";
		if (current == total)
			cerr << " Done.              " << endl;
		else
			cerr << " Waiting for more..." << flush;
	}
}

} //namespace;

int cGcrypt::Init(bool quick_random)
{
	char const *const REQ_GCRYPT_VERSION = "1.2.0";
	if (!gcry_check_version(REQ_GCRYPT_VERSION))
	{
		cerr << "libgcrypt version mismatch\n" << endl;
		return 1;
	}

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
