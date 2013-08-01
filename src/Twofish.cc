//
// Twofish.cc
//
//     Created: 17.01.2012
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

#include "Twofish.hh"

#include <iostream>
#include <stdexcept>

namespace gPWS {

using namespace std;

cTwofish::cTwofish(eMode mode,
                   void const *key, size_t key_len)
{
	gcry_error_t error = gcry_cipher_open(&_h,
	                                      GCRY_CIPHER_TWOFISH,
	                                      mode,
	                                      // FIXME!!!
	                                      0);//GCRY_CIPHER_SECURE);
	if (error)
	{
		cerr << "gcry_cipher_open failed: "
		     << gcry_strsource(error) << "/"
		     << gcry_strerror(error) << endl;
		throw runtime_error("cipher error");
	}

	error = gcry_cipher_setkey(_h, key, key_len);
	if (error)
	{
		cerr << "gcry_cipher_setkey failed: "
		     << gcry_strsource(error) << "/"
		     << gcry_strerror(error) << endl;
		throw runtime_error("cipher error");
	}
}

cTwofish::~cTwofish()
{
	gcry_cipher_close(_h);
}

void cTwofish::SetIV(void const *iv, size_t iv_len)
{
	gcry_error_t error =
		gcry_cipher_setiv(_h, iv, iv_len);
	if (error)
	{
		cerr << "gcry_cipher_setiv failed: "
		     << gcry_strsource(error) << "/"
		     << gcry_strerror(error) << endl;
		throw runtime_error("cipher error");
	}
}

void cTwofish::Decrypt(uint8_t *out, size_t out_len,
                       uint8_t const *in, size_t in_len)
{
	gcry_error_t error =
		gcry_cipher_decrypt(_h, out, out_len, in, in_len);
	if (error)
	{
		cerr << "gcry_cipher_decrypt failed: "
		     << gcry_strsource(error) << "/"
		     << gcry_strerror(error) << endl;
		throw runtime_error("cipher error");
	}
}

void cTwofish::Encrypt(uint8_t *out, size_t out_len,
                       uint8_t const *in, size_t in_len)
{
	gcry_error_t error =
		gcry_cipher_encrypt(_h, out, out_len, in, in_len);
	if (error)
	{
		cerr << "gcry_cipher_encrypt failed: "
		     << gcry_strsource(error) << "/"
		     << gcry_strerror(error) << endl;
		throw runtime_error("cipher error");
	}
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
