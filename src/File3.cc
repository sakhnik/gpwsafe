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
    _file.exceptions(ios::failbit|ios::badbit);
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

    _file.open(fname, _file.in|_file.out|_file.binary);

    char tag[4];
    _file.read(tag, 4);
    if (!std::equal(tag, tag + 4, "PWS3"))
        return -1;

    uint8_t salt[32];
    _file.read(reinterpret_cast<char *>(salt), 32);

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
