//
// main.cc
//
//     Created: 10.01.2012
//      Author: A. Sakhnik
//

#include "File3.hh"
#include "Field.hh"
#include "Debug.hh"

#include <iostream>
#include <gcrypt.h>
#include <boost/format.hpp>

using namespace std;

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
        struct F
        {
            static void Print(gPWS::sField::PtrT const& field)
            {
                assert(field);
                if (field->type == 0xFF)
                {
                    cout << "-------" << endl;
                    return;
                }
                cout << "Length: " << boost::format("%3d") % field->length;
                cout << "\tType: "
                     << boost::format("%02X") % unsigned(field->type);
                cout << "\tValue: "
                     << gPWS::Quote(&field->value[0], field->value.size());
                cout << endl;
            }
        };

        gPWS::cFile3 file;
        if (file.Read("../test/first.psafe3", "first123", &F::Print))
        {
            cerr << "Failed to open file" << endl;
            return 1;
        }
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
