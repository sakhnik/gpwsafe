//
// Entry.hh
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

#pragma once

#include "Field.hh"

#include <functional>
#include <memory>
#include <boost/noncopyable.hpp>

namespace gPWS {

class cEntry
	: boost::noncopyable
{
public:
	typedef std::shared_ptr<cEntry> PtrT;

	static PtrT Create();

	void operator delete(void *p, size_t n);

	cEntry();

	// Deep copy
	cEntry::PtrT Copy() const;

	bool AddField(sField::PtrT const &field);

	typedef std::function<void(sField::PtrT const &ptr)> OnFieldT;

	void ForEachField(OnFieldT on_field);

	void Dump() const;
	void PrettyPrint() const;

	enum eFieldType
	{
		FT_UUID = 0x01,
		FT_GROUP = 0x02,
		FT_TITLE = 0x03,
		FT_USER = 0x04,
		FT_NOTES = 0x05,
		FT_PASS = 0x06,
		FT_URL = 0x0D,

		FT_END = 0xFF
	};

	StringX const &GetValue(eFieldType field_type) const;

	StringX const &GetGroup() const { return GetValue(FT_GROUP); }
	StringX const &GetTitle() const { return GetValue(FT_TITLE); }
	StringX const &GetUser() const  { return GetValue(FT_USER);  }
	StringX const &GetNotes() const { return GetValue(FT_NOTES); }
	StringX const &GetPass() const  { return GetValue(FT_PASS);  }
	StringX const &GetUrl() const   { return GetValue(FT_URL);   }

	void SetValue(eFieldType field_type, StringX const &value);

	void SetGroup(StringX const &value) { SetValue(FT_GROUP, value); }
	void SetTitle(StringX const &value) { SetValue(FT_TITLE, value); }
	void SetUser(StringX const &value)  { SetValue(FT_USER, value);  }
	void SetNotes(StringX const &value) { SetValue(FT_NOTES, value); }
	void SetPass(StringX const &value)  { SetValue(FT_PASS, value);  }
	void SetUrl(StringX const &value)   { SetValue(FT_URL, value);   }

	// group.title
	StringX GetFullTitle() const;

	// Difference from another entry
	struct sChange
	{
		uint8_t type;
		enum eChange
		{
			C_ADDED = 0,
			C_DELETED,
			C_MODIFIED
		} change;
	};

	typedef std::vector<sChange> DiffT;
	DiffT Diff(cEntry::PtrT const &other) const;

private:
	// Associative container. Type is used as index in the vector.
	// Null means the field wasn't defined.
	typedef std::vector<sField::PtrT> _FieldsT;
	_FieldsT _fields;

	// Empty string to refer to if a specific field is missing.
	StringX _empty;

	void *operator new(size_t n);
};

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
