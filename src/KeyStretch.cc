//
// KeyStretch.cc
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

#include "KeyStretch.hh"

namespace gPWS {

using namespace std;

KeyStretch::KeyStretch(void const* pass,
                       unsigned pass_size,
                       void const* salt,
                       unsigned salt_size,
                       unsigned iterations)
{
	_sha256.Update(pass, pass_size);
	_sha256.Update(salt, salt_size);

	for (unsigned i = 0; i != iterations; ++i)
	{
		uint8_t X[Sha256::LENGTH];
		memcpy(X, _sha256.Get(), Sha256::LENGTH);
		_sha256.Reset();
		_sha256.Update(X, Sha256::LENGTH);
	}
}

KeyStretch::~KeyStretch()
{
}

uint8_t const* KeyStretch::Get() const
{
	return _sha256.Get();
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
