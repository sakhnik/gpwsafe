//
// main.cc
//
//     Created: 10.01.2012
//      Author: A. Sakhnik
//

#include "Database.hh"

#include <iostream>
#include <gcrypt.h>

using namespace std;
using namespace gPWS;

int main(int argc, char* argv[])
{
    if (!gcry_check_version(GCRYPT_VERSION))
    {
        cerr << "libgcrypt version mismatch\n" << endl;
        return 1;
    }
    // Allocate secure memory for sensitive information (won't be swapped)
    gcry_control(GCRYCTL_INIT_SECMEM, 16384, 0);

    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);

    try
    {
        cDatabase::PtrT database(new cDatabase);
        database->Read("../test/first.psafe3", "first123");
        database->Dump();
    }
    //catch (fstream::failure const& e)
    //{
    //    cerr << "IO error" << endl;
    //    return 1;
    //}
    catch (std::exception const& e)
    {
        cerr << "Exception: " << e.what() << endl;
        return 1;
    }
    return 0;
}

// vim: set et ts=4 sw=4:
