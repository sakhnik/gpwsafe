//
// File3.cc
//
//     Created: 10.01.2012
//      Author: A. Sakhnik
//

#include "File3.hh"
#include "Defs.hh"

#include <cstring>
#include <iostream>
#include <cassert>
#include <gcrypt.h>

namespace gPWS {

using namespace std;

cFile3::cFile3()
{
    _file.exceptions(_file.failbit|_file.badbit);
}

cFile3::~cFile3()
{
}

int cFile3::Open(char const* fname)
{
    assert(gcry_control(GCRYCTL_INITIALIZATION_FINISHED_P) &&
           "libgcrypt must be initialized beforehand");
    assert(_fname.empty() && "Must be closed before opening");
    _fname = fname;
    assert(!_fname.empty() && "File name?");

    try
    {
        _file.open(fname, _file.in|_file.out|_file.binary);

        const string TAG ("PWS3");
        char tag[TAG.size() + 1];
        _file.read(tag, TAG.size());
        tag[TAG.size()] = 0;
        if (tag != TAG)
            return -1;
    }
    catch (fstream::failure const& e)
    {
        cerr << "Exception opening/reading file" << endl;
        return -1;
    }
    return 0;
}

int cFile3::Close()
{
    if (_fname.empty())
        return -1;
    _fname.clear();
    _file.close();
    return 0;
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
