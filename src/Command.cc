//
// Command.cc
//
//     Created: 26.10.2013
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


#include "Command.hh"
#include "Terminal.hh"
#include "StdoutEmitter.hh"
#include "GtkEmitter.hh"

#include "config.h"

namespace gPWS {

using namespace std;

cCommand::Params::Params()
	: user(false)
	, pass(false)
#if ENABLE_XCLIP && ENABLE_GTK
	, emitter(new cGtkEmitter)
#else
	, emitter(new cStdoutEmitter)
#endif
{
	char const *const DEFAULT_FILE = ".gpwsafe.psafe3";
	char const *home = getenv("HOME");
	if (home)
	{
		file_name = home;
		file_name += "/";
	}
	file_name += DEFAULT_FILE;
}

cDatabase::PtrT cCommand::OpenDatabase(string const &file_name)
{
	cDatabase::PtrT database(new cDatabase);
	string prompt = "Enter password for " + file_name + ": ";
	StringX password = cTerminal::GetPassword(prompt);
	database->Read(file_name, password);
	return database;
}

bool cCommand::CheckSingleEntry(cDatabase::FilterRangeT const &entries)
{
	assert(!entries.empty() && "Must be analyzed separately");

	int count = std::distance(entries.begin(), entries.end());
	if (1 == count)
		return true;

	cerr << "More than one matching entry: ";
	int j = 0;
	for (auto it = entries.begin();
	     it != entries.end() && j != 3; ++it, ++j)
	{
		if (j)
			cerr << ", ";
		cerr << it->first;
	}
	int rest = count - j;
	if (rest)
		cerr << ", ... (" << rest << " more)";
	cerr << " ." << endl;
	return false;
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
