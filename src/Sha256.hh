//
// Sha256.hh
//
//     Created: 14.01.2012
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

#pragma once

#include <gcrypt.h>
#include <stdint.h>
#include <boost/noncopyable.hpp>

namespace gPWS {

class Sha256
	: boost::noncopyable
{
public:
	static const unsigned LENGTH = 32;

	Sha256();
	Sha256(void const *data, size_t len);

	~Sha256();

	void Update(void const *data, size_t len);
	uint8_t const *Get() const;
	void Reset();

private:
	gcry_md_hd_t _h;
};

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
