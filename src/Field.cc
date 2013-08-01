//
// Field.cc
//
//     Created: 21.01.2012
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

#include "Field.hh"
#include "Memory.hh"
#include "Debug.hh"

#include <boost/format.hpp>

namespace gPWS {

using namespace std;

sField::PtrT sField::Create()
{
	return make_shared<sField>();
}

sField::PtrT sField::Copy() const
{
	sField::PtrT field(new sField);
	field->type = type;
	field->value = value;
	return field;
}

void *sField::operator new(size_t n)
{
	return SecureAllocator<sField>::allocate(n);
}

void sField::operator delete(void *p, size_t n)
{
	sField *q(reinterpret_cast<sField *>(p));
	return SecureAllocator<sField>::deallocate(q, n);
}

ostream &operator<<(ostream &os, sField const &field)
{
	boost::format fmt("{0x%02X,%s}");
	fmt % static_cast<unsigned>(field.type);
	fmt % Quote(&field.value[0], field.value.size());
	return os << fmt;
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
