//
// File3.cc
//
//     Created: 10.01.2012
//      Author: A. Sakhnik
//

#include "File3.hh"
#include "KeyStretch.hh"
#include "Memory.hh"

#include <cstring>
#include <iostream>
#include <vector>
#include <cassert>
#include <stdint.h>

namespace gPWS {

using namespace std;

cFile3::cFile3()
{
    _file.exceptions(ios::failbit|ios::badbit);
}

cFile3::~cFile3()
{
}

int cFile3::Open(char const* fname,
                 char const* pass)
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

    const unsigned salt_len = 32;
    char salt[salt_len];
    _file.read(salt, salt_len);

    uint8_t iter_buf[4];
    _file.read(reinterpret_cast<char *>(iter_buf), 4);
    uint32_t iterations = uint32_t(iter_buf[0])
                        + (uint32_t(iter_buf[1]) << 8)
                        + (uint32_t(iter_buf[2]) << 16)
                        + (uint32_t(iter_buf[3]) << 24);

    cKeyStretch key_stretch (pass, strlen(pass),
                             salt, salt_len,
                             iterations);

    cSha256 key_md (key_stretch.Get(), key_stretch.LENGTH);

    vector<char, SecureAllocator<char> > key(32);
    _file.read(&key[0], key.size());

    if (memcmp(&key[0], key_md.Get(), key.size()))
    {
        cerr << "Key mismatch" << endl;
        return -1;
    }

    cout << "Ok" << endl;

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
