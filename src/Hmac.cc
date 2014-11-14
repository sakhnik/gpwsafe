//
// Hmac.cc
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

#include "Hmac.hh"

#include <iostream>
#include <stdexcept>

namespace gPWS {

using namespace std;

Hmac::Hmac(void const *key, size_t key_len)
{
	gcry_error_t error =
		gcry_md_open(&_h,
		             GCRY_MD_SHA256,
		             GCRY_MD_FLAG_SECURE | GCRY_MD_FLAG_HMAC);
	if (error)
	{
		cerr << "gcry_md_open failed: "
		     << gcry_strsource(error) << "/"
		     << gcry_strerror(error) << endl;
		throw runtime_error("md error");
	}

	error = gcry_md_setkey(_h, key, key_len);
	if (error)
	{
		cerr << "gcry_md_setkey failed: "
		     << gcry_strsource(error) << "/"
		     << gcry_strerror(error) << endl;
		throw runtime_error("md error");
	}
}

Hmac::~Hmac()
{
	gcry_md_close(_h);
}

void Hmac::Update(void const *data, size_t len)
{
	gcry_md_write(_h, data, len);
}

uint8_t const *Hmac::Get() const
{
	return gcry_md_read(_h, 0);
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
