//
// File3.cc
//
//     Created: 10.01.2012
//      Author: A. Sakhnik
//
// This file is part of gpwsafe.
//
// Gpwsafe is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Gpwsafe is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with gpwsafe.  If not, see <http://www.gnu.org/licenses/>

#include "File3.hh"
#include "Defs.hh"
#include "KeyStretch.hh"
#include "Memory.hh"
#include "Random.hh"
#include "Debug.hh"

#include <stdint.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <cassert>
#include <stdexcept>

namespace gPWS {

using namespace std;

const string MAGIC_TAG("PWS3");
const unsigned SALT_LEN = 32;
const string EOF_TAG("PWS3-EOFPWS3-EOF");

cFile3::cFile3()
    : _state(S_CLOSED)
    , _initial_state(_fs.exceptions())
    , _iterations(2048)
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
                      StringX const &pass)
{
    assert(_state == S_CLOSED);
    _state = S_READING;

    _fs.exceptions(_initial_state);
    _fs.open(fname, ios::in|ios::binary);
    if (!_fs)
        throw runtime_error("Can't open file for reading");
    _fs.exceptions(ios::failbit|ios::badbit);

    char tag[MAGIC_TAG.size()];
    _fs.read(tag, MAGIC_TAG.size());
    if (!std::equal(MAGIC_TAG.begin(), MAGIC_TAG.end(), tag))
        throw runtime_error("Invalid tag");

    char salt[SALT_LEN];
    _fs.read(salt, SALT_LEN);

    uint8_t iter_buf[4];
    _fs.read(reinterpret_cast<char *>(iter_buf), 4);
    _iterations = uint32_t(iter_buf[0])
                + (uint32_t(iter_buf[1]) << 8)
                + (uint32_t(iter_buf[2]) << 16)
                + (uint32_t(iter_buf[3]) << 24);

    cKeyStretch key_stretch(pass.c_str(), pass.size(),
                            salt, SALT_LEN,
                            _iterations);

    cSha256 key_md(key_stretch.Get(), key_stretch.LENGTH);

    string key(cSha256::LENGTH, '\0');
    _fs.read(&key[0], key.size());

    if (memcmp(&key[0], key_md.Get(), key.size()))
        throw runtime_error("Key mismatch");

    cTwofish twofish(cTwofish::M_ECB, key_stretch.Get(), key_stretch.LENGTH);

    if (_main_key.empty())
        _main_key.resize(cTwofish::KEY_LENGTH);
    _fs.read(&_main_key[0], _main_key.size());
    twofish.Decrypt(&_main_key[0], _main_key.size(),
                    &_main_key[0], _main_key.size());

    if (_hmac_key.empty())
        _hmac_key.resize(cHmac::KEY_LENGTH);
    _fs.read(&_hmac_key[0], _hmac_key.size());
    twofish.Decrypt(&_hmac_key[0], _hmac_key.size(),
                    &_hmac_key[0], _hmac_key.size());
    _hmac_calculator.reset(new cHmac(&_hmac_key[0], _hmac_key.size()));

    BytesT iv(cTwofish::BLOCK_LENGTH);
    _fs.read(reinterpret_cast<char *>(&iv[0]), iv.size());

    _twofish.reset(new cTwofish(cTwofish::M_CBC,
                                &_main_key[0],
                                _main_key.size()));
    _twofish->SetIV(&iv[0], iv.size());
}

sField::PtrT cFile3::ReadField()
{
    assert(_state == S_READING);

    _data.resize(cTwofish::BLOCK_LENGTH);
    _fs.read(&_data[0], _data.size());

    assert(EOF_TAG.size() == _data.size());
    if (!memcmp(&_data[0], EOF_TAG.data(), _data.size()))
    {
        BytesT hmac(cHmac::LENGTH);
        _fs.read(reinterpret_cast<char *>(&hmac[0]), hmac.size());
        _state = S_CLOSED;
        _fs.close();
        if (memcmp(_hmac_calculator->Get(), &hmac[0], hmac.size()))
            throw runtime_error("HMAC check failed");
        return sField::PtrT();
    }

    _twofish->Decrypt(&_data[0], _data.size(), &_data[0], _data.size());
    uint8_t const *udata = reinterpret_cast<uint8_t const *>(&_data[0]);
    uint32_t length = uint32_t(udata[0])
                    | (uint32_t(udata[1]) << 8)
                    | (uint32_t(udata[2]) << 16)
                    | (uint32_t(udata[3]) << 24);

    sField::PtrT field(sField::Create());
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
        int available = (std::min)(_data.size() - i, size_t(length));
        value.insert(value.end(),
                     _data.begin() + i,
                     _data.begin() + i + available);
        length -= available;
        i += available;
    }
    _hmac_calculator->Update(&value[0], value.size());
    return field;
}

void cFile3::OpenWrite(char const *fname,
                       StringX const &pass,
                       bool new_keys,
                       bool very_strong)
{
    assert(_state == S_CLOSED);
    _state = S_WRITING;

    _fs.exceptions(_initial_state);
    _fs.open(fname, ios::out|ios::binary|ios::trunc);
    if (!_fs)
        throw runtime_error("Can't open file for writing");
    _fs.exceptions(ios::failbit|ios::badbit);

    _fs.write(&MAGIC_TAG[0], MAGIC_TAG.size());

    char salt[SALT_LEN];
    cRandom::CreateNonce(salt, SALT_LEN);
    _fs.write(salt, SALT_LEN);

    uint8_t iter_buf[4];
    iter_buf[0] = _iterations & 0xFF;
    iter_buf[1] = (_iterations >> 8) & 0xFF;
    iter_buf[2] = (_iterations >> 16) & 0xFF;
    iter_buf[3] = (_iterations >> 24) & 0xFF;

    _fs.write(reinterpret_cast<char *>(iter_buf), 4);

    cKeyStretch key_stretch(pass.c_str(), pass.size(),
                            salt, SALT_LEN,
                            _iterations);

    cSha256 key_md(key_stretch.Get(), key_stretch.LENGTH);

    _fs.write(reinterpret_cast<char const *>(key_md.Get()), key_md.LENGTH);

    cTwofish twofish(cTwofish::M_ECB, key_stretch.Get(), key_stretch.LENGTH);

    if (_main_key.empty() || new_keys)
    {
        _main_key.resize(cTwofish::KEY_LENGTH);
        cRandom::Randomize(&_main_key[0], _main_key.size(), very_strong);
    }
    // Encrypt the main key before writing
    StringX main_key_enc(_main_key);
    twofish.Encrypt(&main_key_enc[0], main_key_enc.size(),
                    &_main_key[0], _main_key.size());
    _fs.write(&main_key_enc[0], main_key_enc.size());

    if (_hmac_key.empty() || new_keys)
    {
        _hmac_key.resize(cHmac::KEY_LENGTH);
        cRandom::Randomize(&_hmac_key[0], _hmac_key.size(), very_strong);
    }
    StringX hmac_key_enc(_hmac_key);
    twofish.Encrypt(&hmac_key_enc[0], hmac_key_enc.size(),
                    &_hmac_key[0], _hmac_key.size());
    _fs.write(&hmac_key_enc[0], hmac_key_enc.size());

    _hmac_calculator.reset(new cHmac(&_hmac_key[0], _hmac_key.size()));

    BytesT iv(cTwofish::BLOCK_LENGTH);
    cRandom::CreateNonce(&iv[0], iv.size());
    _fs.write(reinterpret_cast<char *>(&iv[0]), iv.size());

    _twofish.reset(new cTwofish(cTwofish::M_CBC,
                                &_main_key[0],
                                _main_key.size()));
    _twofish->SetIV(&iv[0], iv.size());
}

void cFile3::WriteField(sField::PtrT const &field)
{
    assert(_state == S_WRITING);
    assert(field);

    _data.resize(cTwofish::BLOCK_LENGTH);

    StringX &value(field->value);
    uint32_t length(value.size());
    _hmac_calculator->Update(&value[0], value.size());

    _data[0] = length & 0xFF;
    _data[1] = (length >> 8) & 0xFF;
    _data[2] = (length >> 16) & 0xFF;
    _data[3] = (length >> 24) & 0xFF;
    _data[4] = field->type;

    for (unsigned val_idx = 0, data_idx = 5; ; )
    {
        int data_rest = _data.size() - data_idx;
        int value_rest = value.size() - val_idx;
        int block_rest = (std::min)(data_rest, value_rest);
        if (!value_rest && data_rest)
        {
            // Fill up the rest of the block with random data
            cRandom::CreateNonce(&_data[0] + data_idx, data_rest);
        }
        if (!block_rest)
        {
            assert(_twofish.get());
            _twofish->Encrypt(&_data[0], _data.size(),
                              &_data[0], _data.size());
            _fs.write(&_data[0], _data.size());
            data_idx = 0;
            if (!value_rest)
                break;
            continue;
        }
        copy(value.begin() + val_idx, value.begin() + val_idx + block_rest,
             _data.begin() + data_idx);
        data_idx += block_rest;
        val_idx += block_rest;
    }
}

void cFile3::CloseWrite()
{
    assert(_state == S_WRITING);
    _fs.write(EOF_TAG.data(), EOF_TAG.size());
    assert(_hmac_calculator.get());
    _fs.write(reinterpret_cast<char const *>(_hmac_calculator->Get()),
              cHmac::LENGTH);
    _fs.close();
    _state = S_CLOSED;
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
