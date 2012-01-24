//
// main.cc
//
//     Created: 10.01.2012
//      Author: A. Sakhnik
//

#include "../config.h"
#include "App.hh"

#include <sys/stat.h>
#include <iostream>
#include <gcrypt.h>

using namespace std;
using namespace gPWS;

static char const *_Basename(char const *path)
{
    char const *res = strrchr(path, '/');
    if (!res)
        return path;
    return res + 1;
}

int main(int argc, char* argv[])
{
    char const *program_name = _Basename(argv[0]);

    // Be nice and paranoid
    umask(0077);

    char const *const REQ_GCRYPT_VERSION = "1.2.0";
    if (!gcry_check_version(REQ_GCRYPT_VERSION))
    {
        cerr << "libgcrypt version mismatch\n" << endl;
        return 1;
    }
    // Allocate secure memory for sensitive information (won't be swapped)
    gcry_control(GCRYCTL_INIT_SECMEM, 16384, 0);
    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);

    cApp app(program_name);
    if (int res = app.Init(argc, argv))
        return res;

    return app.Run();
}

// vim: set et ts=4 sw=4:
