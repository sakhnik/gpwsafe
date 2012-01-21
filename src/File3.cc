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

#include <stdint.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <cassert>
#include <stdexcept>

namespace gPWS {

using namespace std;

cFile3::cFile3()
    : _initial_state(_fs.exceptions())
{
    assert(gcry_control(GCRYCTL_INITIALIZATION_FINISHED_P) &&
           "libgcrypt must be initialized beforehand");
}

cFile3::~cFile3()
{
}

void cFile3::Close()
{
    _state = S_CLOSED;
    _fs.close();
}

void cFile3::OpenRead(char const *fname,
                      char const *pass)
{
    assert(_state == S_CLOSED);
    _state = S_READING;

    _fs.exceptions(_initial_state);
    _fs.open(fname, ios::in|ios::binary);
    if (!_fs)
        throw runtime_error("Can't open file");
    _fs.exceptions(ios::failbit|ios::badbit);

    char tag[4];
    _fs.read(tag, 4);
    if (!std::equal(tag, tag + 4, "PWS3"))
        throw runtime_error("Invalid tag");

    const unsigned salt_len = 32;
    char salt[salt_len];
    _fs.read(salt, salt_len);

    uint8_t iter_buf[4];
    _fs.read(reinterpret_cast<char *>(iter_buf), 4);
    uint32_t iterations = uint32_t(iter_buf[0])
                        + (uint32_t(iter_buf[1]) << 8)
                        + (uint32_t(iter_buf[2]) << 16)
                        + (uint32_t(iter_buf[3]) << 24);

    cKeyStretch key_stretch(pass, strlen(pass),
                            salt, salt_len,
                            iterations);

    cSha256 key_md(key_stretch.Get(), key_stretch.LENGTH);

    string key(32, '\0');
    _fs.read(&key[0], key.size());

    if (memcmp(&key[0], key_md.Get(), key.size()))
        throw runtime_error("Key mismatch");

    cTwofish twofish(cTwofish::M_ECB, key_stretch.Get(), key_stretch.LENGTH);

    StringX main_key(32, '\0');
    _fs.read(&main_key[0], main_key.size());
    twofish.Decrypt(&main_key[0], main_key.size(),
                    &main_key[0], main_key.size());

    StringX hmac_key(32, '\0');
    _fs.read(&hmac_key[0], hmac_key.size());
    twofish.Decrypt(&hmac_key[0], hmac_key.size(),
                    &hmac_key[0], hmac_key.size());
    _hmac_calculator.reset(new cHmac(&hmac_key[0], hmac_key.size()));

    BytesT iv(16);
    _fs.read(reinterpret_cast<char *>(&iv[0]), iv.size());

    _twofish.reset(new cTwofish(cTwofish::M_CBC,
                                &main_key[0],
                                main_key.size()));
    _twofish->SetIV(&iv[0], iv.size());
}

sField::PtrT cFile3::ReadField()
{
    assert(_state == S_READING);

    _data.resize(16);
    _fs.read(&_data[0], _data.size());

    if (!memcmp(&_data[0], "PWS3-EOFPWS3-EOF", _data.size()))
    {
        BytesT hmac(32);
        _fs.read(reinterpret_cast<char *>(&hmac[0]), hmac.size());
        _state = S_CLOSED;
        _fs.close();
        if (memcmp(_hmac_calculator->Get(), &hmac[0], hmac.size()))
            throw runtime_error("HMAC check failed");
        return sField::PtrT();
    }

    _twofish->Decrypt(&_data[0], _data.size(), &_data[0], _data.size());
    uint32_t length = uint32_t(_data[0])
                    | (uint32_t(_data[1]) << 8)
                    | (uint32_t(_data[2]) << 16)
                    | (uint32_t(_data[3]) << 24);

    sField::PtrT field(new sField);
    field->length = length;
    field->type = _data[4];
    field->value.reserve(length);
    StringX &value (field->value);

    for (unsigned i = 5; length > 0; )
    {
        if (i == _data.size())
        {
            _fs.read(&_data[0], _data.size());
            _twofish->Decrypt(&_data[0], _data.size(),
                              &_data[0], _data.size());
            i = 0;
        }
        int available = (std::min)(_data.size() - i, length);
        value.insert(value.end(),
                     _data.begin() + i,
                     _data.begin() + i + available);
        length -= available;
        i += available;
    }
    _hmac_calculator->Update(&value[0], value.size());
    return field;
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
