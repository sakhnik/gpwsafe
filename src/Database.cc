//
// Database.cc
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

#include "Database.hh"
#include "Memory.hh"
#include "Debug.hh"
#include "i18n.h"

#include <iostream>
#include <boost/format.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/filesystem.hpp>

namespace gPWS {

using namespace std;

void *Database::operator new(size_t n)
{
	return SecureAllocator<Database>::allocate(n);
}

void Database::operator delete(void *p, size_t n)
{
	Database *q = reinterpret_cast<Database *>(p);
	return SecureAllocator<Database>::deallocate(q, n);
}

Database::Database()
	: _changed(false)
{
	_fields.reserve(0x0F);
}

void Database::Create()
{
	_fields.clear();
	_changed = false;

	sField::PtrT field(sField::Create());

	// Insert default version (mock 3.10)
	field->type = FT_VERSION;
	field->value.clear();
	field->value.push_back(0x0A);
	field->value.push_back(0x03);
	_AddField(field);

	// Insert new UUID
	field->type = FT_UUID;
	field->value.resize(16);
	boost::uuids::uuid new_uuid = boost::uuids::random_generator()();
	assert(new_uuid.size() == 16);
	copy(new_uuid.begin(), new_uuid.end(), field->value.begin());
	_AddField(field);
}

void Database::Read(string const &fname, StringX const &pass)
{
	// The database must be clear.
	assert(!_changed && "The changes must be written beforehand");

	_changed = false;
	_fname = fname;
	_pass = pass;
	_file.OpenRead(fname.c_str(), pass);

	// Read header fields first
	sField::PtrT field;
	while ((field = _file.ReadField()))
	{
		if (!_AddField(field))
			break;
	}

	if (!field)
		return;

	Entry::PtrT entry(Entry::Create());
	while ((field = _file.ReadField()))
	{
		if (!entry->AddField(field))
		{
			// FIXME: Check if such key already exists.
			// Avoid loosing information.
			_entries[entry->GetFullTitle()] = entry;
			entry = Entry::Create();
		}
	}
}

void Database::Write(string const &fname, StringX const &pass)
{
	_changed = false;
	_file.OpenWrite(fname.c_str(), pass, true);

	for (auto &field : _fields)
	{
		if (field)
			_file.WriteField(field);
	}

	// Field terminator
	sField::PtrT terminator(sField::Create());
	terminator->type = 0xFF;
	terminator->value.clear();
	_file.WriteField(terminator);

	for (auto &title_entry : _entries)
	{
		auto &entry = title_entry.second;
		entry->ForEachField([this](sField::PtrT const &field) {
							_file.WriteField(field);
							});
		_file.WriteField(terminator);
	}

	_file.CloseWrite();
}

void Database::Write()
{
	assert(!_fname.empty() && !_pass.empty());

	using namespace boost::filesystem;

	auto target = canonical(_fname);
	auto new_fname = target;
	new_fname += ".new";
	auto backup = target;
	backup += '~';

	remove(new_fname);
	Write(new_fname.string(), _pass);

	// The following calls are unlikely to fail.
	remove(backup);

	rename(target, backup);
	rename(new_fname, target);
}

bool Database::_AddField(sField::PtrT const &field)
{
	if (field->type == 0xFF)
		return false;
	if (_fields.size() <= field->type)
		_fields.resize(static_cast<unsigned>(field->type) + 1);
	_fields[field->type] = field;
	return true;
}

void Database::Dump() const
{
	for (auto const &field : _fields)
	{
		if (!field)
			continue;
		cout << "{0x"
		     << boost::format("%02X") % unsigned(field->type)
		     << ", '"
		     << gPWS::Quote(&field->value[0], field->value.size())
		     << "'}\n";
	}
	cout << "==========" << endl;

	for (auto const &keyval : _entries)
	{
		auto const &entry = keyval.second;
		entry->Dump();
	}
}

void Database::AddEntry(Entry::PtrT const &entry)
{
	// FIXME: Check if such key already exists. Avoid loosing information.
	_entries[entry->GetFullTitle()] = entry;
	_changed = true;
}

void Database::RemoveEntry(Entry::PtrT const &entry)
{
	_entries.erase(entry->GetFullTitle());
	_changed = true;
}

bool Database::HasEntry(StringX const &full_title) const
{
	return _entries.find(full_title) != _entries.end();
}

Database::MatchT Database::Find(char const *query) const
{
	MatchT match;
	for (auto first = begin(_entries), last = end(_entries);
	     first != last;
	     ++first)
	{
		if (!query || first->first.find(query) != first->first.npos)
			match.push_back(first);
	}
	return match;
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
