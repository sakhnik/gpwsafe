//
// Command.cc
//
//     Created: 26.10.2013
//      Author: A. Sakhnik
//

#include "Command.hh"
#include "Terminal.hh"
#include "StdoutEmitter.hh"
#include "GtkEmitter.hh"

namespace gPWS {

using namespace std;

cCommand::Params::Params()
	: user(false)
	, pass(false)
#if defined(ENABLE_XCLIP) && defined(ENABLE_GTK)
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
