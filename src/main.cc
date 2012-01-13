//
// main.cc
//
//     Created: 10.01.2012
//      Author: A. Sakhnik
//

#include "File3.hh"

#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    try
    {
        gPWS::cFile3 file;
        if (file.Open("../test/first.psafe3"))
        {
            cerr << "Failed to open file" << endl;
            return 1;
        }

        if (file.Close())
        {
            cerr << "Failed to close file" << endl;
            return 1;
        }
    }
    catch (std::exception const& e)
    {
        cerr << "Exception: " << e.what() << endl;
    }
    return 0;
}

// vim: set et ts=4 sw=4:
