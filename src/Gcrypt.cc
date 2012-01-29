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

static void
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

int cGcrypt::Init()
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
    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);

    return 0;
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
