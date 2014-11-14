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
#include "i18n.h"

#include <iostream>
#include <boost/format.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace gPWS {

using namespace std;

Entry::PtrT Entry::Create()
{
	return make_shared<Entry>();
}

void *Entry::operator new(size_t n)
{
	return SecureAllocator<Entry>::allocate(n);
}

void Entry::operator delete(void *p, size_t n)
{
	Entry *q = reinterpret_cast<Entry *>(p);
	return SecureAllocator<Entry>::deallocate(q, n);
}

Entry::Entry()
	: _fields(0x0F, sField::PtrT())
{
}

Entry::PtrT Entry::Copy() const
{
	Entry::PtrT entry(new Entry);
	entry->_fields.resize(_fields.size());
	for (unsigned i = 0; i != _fields.size(); ++i)
	{
		if (_fields[i])
			entry->_fields[i] = _fields[i]->Copy();
	}
	return entry;
}

bool Entry::AddField(sField::PtrT const &field)
{
	if (field->type == FT_END)
		return false;
	if (_fields.size() <= field->type)
		_fields.resize(static_cast<unsigned>(field->type) + 1);
	_fields[field->type] = field;
	return true;
}

void Entry::ForEachField(OnFieldT on_field)
{
	for (auto &field : _fields)
	{
		if (!field)
			continue;
		on_field(field);
	}
}

void Entry::Dump() const
{
	for (auto &field : _fields)
	{
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

void Entry::PrettyPrint() const
{
	for (auto &field : _fields)
	{
		if (!field)
			continue;
		switch (field->type)
		{
		case FT_UUID:
			{
				assert(field->value.size() == 16);
				boost::uuids::uuid u;
				memcpy(&u, &field->value[0], 16);
				cout << "UUID:\t\t" << u << "\n";
				break;
			}
		case FT_GROUP:
			cout << _("Group:\t\t") << field->value << "\n";
			break;
		case FT_TITLE:
			cout << _("Title:\t\t") << field->value << "\n";
			break;
		case FT_USER:
			cout << _("User:\t\t") << field->value << "\n";
			break;
		case FT_NOTES:
			cout << _("Notes:\t\t") << field->value << "\n";
			break;
		case FT_PASS:
			cout << _("Pass:\t\t*************\n");
			break;
		case FT_URL:
			cout << "URL:\t\t" << field->value << "\n";
			break;
		default:
			cout << boost::format("0x%02X") % unsigned(field->type) << "\t\t";
			cout << "'" << gPWS::Quote(&field->value[0], field->value.size());
			cout << "'\n";
			break;
		}
	}
}

StringX const &Entry::GetValue(eFieldType field_type) const
{
	if (static_cast<size_t>(field_type) >= _fields.size())
		return _empty;
	sField::PtrT const &field = _fields[field_type];
	if (!field)
		return _empty;
	assert(field_type == field->type);
	return field->value;
}

StringX Entry::GetFullTitle() const
{
	if (GetGroup().empty())
		return GetTitle();
	return GetGroup() + "." + GetTitle();
}

void Entry::SetValue(eFieldType field_type, StringX const &value)
{
	if (static_cast<size_t>(field_type) >= _fields.size())
		_fields.resize(static_cast<unsigned>(field_type) + 1);
	sField::PtrT &field = _fields[field_type];
	if (!field)
		field = sField::Create();
	field->type = field_type;
	field->value = value;
}

Entry::DiffT
Entry::Diff(Entry::PtrT const &other) const
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
