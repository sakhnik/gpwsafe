//
// Database.hh
//
//     Created: 20.01.2012
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

#include "File3.hh"
#include "Entry.hh"

#include <stdint.h>
#include <map>
#include <memory>
#include <vector>
#include <boost/noncopyable.hpp>

namespace gPWS {

class Database
	: boost::noncopyable
{
public:
	typedef std::shared_ptr<Database> PtrT;

	void *operator new(size_t n);
	void operator delete(void *p, size_t n);

	Database();

	void Create();

	void Read(std::string const &fname,
	          StringX const &pass);

	void Write(std::string const &fname,
	           StringX const &pass);

	// Use the cached file name and password
	void Write();

	void Dump() const;

	void AddEntry(Entry::PtrT const &entry);
	void RemoveEntry(Entry::PtrT const &entry);
	bool HasEntry(StringX const &full_title) const;

private:
	File3 _file;
	bool _changed;
	std::string _fname; // Cached file name
	StringX _pass;      // Cached password

	enum _eFieldType
	{
		FT_VERSION = 0x00,
		FT_UUID = 0x01,

		FT_END = 0xFF
	};

	// Associative vector of fields
	typedef std::vector<sField::PtrT> _FieldsT;
	// Database header
	_FieldsT _fields;

	// FullTitle -> EntryT
	typedef std::map<StringX, Entry::PtrT> _TitleEntryT;
	_TitleEntryT _entries;

	bool _AddField(sField::PtrT const &field);

public:
	typedef std::vector<_TitleEntryT::const_iterator> MatchT;

	MatchT Find(char const *query) const;
};

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
