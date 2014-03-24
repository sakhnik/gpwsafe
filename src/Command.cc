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
#include "i18n.h"

#include "config.h"
#include <wordexp.h>
#include <boost/scope_exit.hpp>
#include <boost/format.hpp>

namespace gPWS {

using namespace std;
typedef boost::format bfmt;

cCommand::Params::Params()
	: file_name("~/.gpwsafe.psafe3")
	, user(false)
	, pass(false)
#if ENABLE_XCLIP && ENABLE_GTK
	, emitter(new cGtkEmitter)
#else
	, emitter(new cStdoutEmitter)
#endif
	, selection("BOTH")
{
}

string cCommand::Params::ExpandFileName() const
{
	wordexp_t exp_result = { };
	BOOST_SCOPE_EXIT((&exp_result)) {
		wordfree(&exp_result);
	} BOOST_SCOPE_EXIT_END

	wordexp(file_name.c_str(), &exp_result, 0);
	return exp_result.we_wordv[0];
}

cDatabase::PtrT cCommand::OpenDatabase(string const &file_name)
{
	cDatabase::PtrT database(new cDatabase);
	string prompt = _("Enter password for ") + file_name + ": ";
	StringX password = cTerminal::GetPassword(prompt);
	database->Read(file_name, password);
	return database;
}

bool cCommand::CheckSingleEntry(cDatabase::MatchT const &entries)
{
	assert(!entries.empty() && "Must be analyzed separately");

	if (1 == entries.size())
		return true;

	cerr << _("More than one matching entry: ");
	int j = 0;
	for (auto it = begin(entries);
	     it != end(entries) && j != 3; ++it, ++j)
	{
		if (j)
			cerr << ", ";
		cerr << (*it)->first;
	}
	int rest = entries.size() - j;
	if (rest)
		cerr << ", ... (" << bfmt(_("%d more")) % rest << ")";
	cerr << " ." << endl;
	return false;
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
