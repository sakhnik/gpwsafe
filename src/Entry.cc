//
// Entry.cc
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

#include "Entry.hh"
#include "Memory.hh"
#include "Debug.hh"

#include <iostream>
#include <boost/format.hpp>

namespace gPWS {

using namespace std;

cEntry::PtrT cEntry::Create()
{
	return make_shared<cEntry>();
}

void *cEntry::operator new(size_t n)
{
	return SecureAllocator<cEntry>::allocate(n);
}

void cEntry::operator delete(void *p, size_t n)
{
	cEntry *q = reinterpret_cast<cEntry *>(p);
	return SecureAllocator<cEntry>::deallocate(q, n);
}

cEntry::cEntry()
	: _fields(0x0F, sField::PtrT())
{
}

cEntry::PtrT cEntry::Copy() const
{
	cEntry::PtrT entry(new cEntry);
	entry->_fields.resize(_fields.size());
	for (unsigned i = 0; i != _fields.size(); ++i)
	{
		if (_fields[i])
			entry->_fields[i] = _fields[i]->Copy();
	}
	return entry;
}

bool cEntry::AddField(sField::PtrT const &field)
{
	if (field->type == FT_END)
		return false;
	if (_fields.size() <= field->type)
		_fields.resize(static_cast<unsigned>(field->type) + 1);
	_fields[field->type] = field;
	return true;
}

void cEntry::ForEachField(OnFieldT on_field)
{
	for (_FieldsT::const_iterator i = _fields.begin();
	     i != _fields.end(); ++i)
	{
		sField::PtrT const &field(*i);
		if (!field)
			continue;
		on_field(field);
	}
}

void cEntry::Dump() const
{
	for (_FieldsT::const_iterator i = _fields.begin();
	     i != _fields.end(); ++i)
	{
		sField::PtrT const& field = *i;
		if (!field)
			continue;
		cout << "{0x"
			<< boost::format("%02X") % unsigned(field->type)
			<< ", '"
			<< gPWS::Quote(&field->value[0], field->value.size())
			<< "'}"
			<< endl;
	}
	cout << "----------" << endl;
}

StringX const &cEntry::GetValue(eFieldType field_type) const
{
	if (static_cast<size_t>(field_type) >= _fields.size())
		return _empty;
	sField::PtrT const &field = _fields[field_type];
	if (!field)
		return _empty;
	assert(field_type == field->type);
	return field->value;
}

StringX cEntry::GetFullTitle() const
{
	if (GetGroup().empty())
		return GetTitle();
	return GetGroup() + "." + GetTitle();
}

void cEntry::SetValue(eFieldType field_type, StringX const &value)
{
	if (static_cast<size_t>(field_type) >= _fields.size())
		_fields.resize(static_cast<unsigned>(field_type) + 1);
	sField::PtrT &field = _fields[field_type];
	if (!field)
		field = sField::Create();
	field->type = field_type;
	field->value = value;
}

cEntry::DiffT
cEntry::Diff(cEntry::PtrT const &other) const
{
	DiffT diff;
	for (unsigned i = 0, n = (std::max)(_fields.size(), other->_fields.size());
	     i != n; ++i)
	{
		sField::PtrT field;
		if (i < _fields.size())
			field = _fields[i];
		sField::PtrT field_o;
		if (i < other->_fields.size())
			field_o = other->_fields[i];

		if (field)
		{
			if (!field_o)
			{
				sChange change = { field->type, sChange::C_ADDED };
				diff.push_back(change);
			}
			else
			{
				if (field->value != field_o->value)
				{
					sChange change = { field->type, sChange::C_MODIFIED };
					diff.push_back(change);
				}
			}
		}
		else // !field
		{
			if (field_o)
			{
				sChange change = { field_o->type, sChange::C_DELETED };
				diff.push_back(change);
			}
		}
	}
	return diff;
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
