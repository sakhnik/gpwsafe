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
#include "Hmac.hh"

#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <stdint.h>

namespace gPWS {

using namespace std;

cFile3::cFile3()
{
    assert(gcry_control(GCRYCTL_INITIALIZATION_FINISHED_P) &&
           "libgcrypt must be initialized beforehand");
}

cFile3::~cFile3()
{
}

int cFile3::Read(char const *fname,
                 char const *pass,
                 OnFieldT const &on_field)
{
    ifstream ifs(fname, ios::in|ios::binary);
    if (!ifs)
    {
        cerr << "Can't open file" << endl;
        return -1;
    }
    ifs.exceptions(ios::failbit|ios::badbit);

    char tag[4];
    ifs.read(tag, 4);
    if (!std::equal(tag, tag + 4, "PWS3"))
        return -1;

    const unsigned salt_len = 32;
    char salt[salt_len];
    ifs.read(salt, salt_len);

    uint8_t iter_buf[4];
    ifs.read(reinterpret_cast<char *>(iter_buf), 4);
    uint32_t iterations = uint32_t(iter_buf[0])
                        + (uint32_t(iter_buf[1]) << 8)
                        + (uint32_t(iter_buf[2]) << 16)
                        + (uint32_t(iter_buf[3]) << 24);

    cKeyStretch key_stretch(pass, strlen(pass),
                            salt, salt_len,
                            iterations);

    cSha256 key_md(key_stretch.Get(), key_stretch.LENGTH);

    BytesX key(32);
    ifs.read(reinterpret_cast<char *>(&key[0]), key.size());

    if (memcmp(&key[0], key_md.Get(), key.size()))
    {
        cerr << "Key mismatch" << endl;
        return -1;
    }

    cTwofish twofish(cTwofish::M_ECB, key_stretch.Get(), key_stretch.LENGTH);

    BytesX main_key(32);
    ifs.read(reinterpret_cast<char *>(&main_key[0]), main_key.size());
    twofish.Decrypt(&main_key[0], main_key.size(),
                    &main_key[0], main_key.size());

    BytesX hmac_key(32);
    ifs.read(reinterpret_cast<char *>(&hmac_key[0]), hmac_key.size());
    twofish.Decrypt(&hmac_key[0], hmac_key.size(),
                    &hmac_key[0], hmac_key.size());
    cHmac hmac_calculator(&hmac_key[0], hmac_key.size());

    BytesT iv(16);
    ifs.read(reinterpret_cast<char *>(&iv[0]), iv.size());

    cTwofish twofish2(cTwofish::M_CBC, &main_key[0], main_key.size());
    twofish2.SetIV(&iv[0], iv.size());
    BytesX data(16);
    while (true)
    {
        ifs.read(reinterpret_cast<char *>(&data[0]), data.size());
        if (!memcmp(&data[0], "PWS3-EOFPWS3-EOF", data.size()))
            break;
        twofish2.Decrypt(&data[0], data.size(), &data[0], data.size());
        uint32_t length = uint32_t(data[0])
                        | (uint32_t(data[1]) << 8)
                        | (uint32_t(data[2]) << 16)
                        | (uint32_t(data[3]) << 24);
        if (!length)
        {
            if (on_field)
                on_field(sField::PtrT());
            continue;
        }

        sField::PtrT field(new sField);
        field->length = length;
        field->type = data[4];
        field->value.reserve(length);
        BytesX &value (field->value);

        for (unsigned i = 5; length > 0; )
        {
            if (i == data.size())
            {
                ifs.read(reinterpret_cast<char *>(&data[0]), data.size());
                twofish2.Decrypt(&data[0], data.size(), &data[0], data.size());
                i = 0;
            }
            int available = (std::min)(data.size() - i, length);
            value.insert(value.end(),
                         data.begin() + i,
                         data.begin() + i + available);
            length -= available;
            i += available;
        }
        if (on_field)
            on_field(field);
        hmac_calculator.Update(&value[0], value.size());
    }

    BytesT hmac(32);
    ifs.read(reinterpret_cast<char *>(&hmac[0]), hmac.size());

    if (memcmp(hmac_calculator.Get(), &hmac[0], hmac.size()))
    {
        cerr << "HMAC check failed" << endl;
        return -1;
    }

    return 0;
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
