//
// File3.cc
//
//     Created: 10.01.2012
//      Author: A. Sakhnik
//

#include "File3.hh"
#include "Defs.hh"
#include "KeyStretch.hh"
#include "Memory.hh"
#include "Debug.hh"
#include "Twofish.hh"

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

int cFile3::Open(char const *fname,
                 char const *pass)
{
    assert(gcry_control(GCRYCTL_INITIALIZATION_FINISHED_P) &&
           "libgcrypt must be initialized beforehand");

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

    cKeyStretch key_stretch(pass, strlen(pass),
                            salt, salt_len,
                            iterations);

    cSha256 key_md(key_stretch.Get(), key_stretch.LENGTH);

    SecureBytesT key(32);
    _file.read(reinterpret_cast<char *>(&key[0]), key.size());

    if (memcmp(&key[0], key_md.Get(), key.size()))
    {
        cerr << "Key mismatch" << endl;
        return -1;
    }

    SecureBytesT main_key(32);
    _file.read(reinterpret_cast<char *>(&main_key[0]), main_key.size());
    cTwofish twofish(cTwofish::M_ECB, key_stretch.Get(), key_stretch.LENGTH);
    twofish.Decrypt(&main_key[0], main_key.size(),
                    &main_key[0], main_key.size());

    SecureBytesT hmac_key(32);
    _file.read(reinterpret_cast<char *>(&hmac_key[0]), hmac_key.size());
    twofish.Decrypt(&hmac_key[0], hmac_key.size(),
                    &hmac_key[0], hmac_key.size());

    BytesT iv(16);
    _file.read(reinterpret_cast<char *>(&iv[0]), iv.size());

    cTwofish twofish2(cTwofish::M_CBC, &main_key[0], main_key.size());
    twofish2.SetIV(&iv[0], iv.size());

    BytesT data(1024);
    _file.read(reinterpret_cast<char *>(&data[0]), data.size());
    twofish2.Decrypt(&data[0], data.size(), &data[0], data.size());

    std::copy(data.begin(), data.end(),
              ostream_iterator<BytesT::value_type>(cout));

    return 0;
}

int cFile3::Close()
{
    _file.close();
    return 0;
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
