//
// App.cc
//
//     Created: 24.01.2012
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

#include "App.hh"
#include "Database.hh"
#include "Terminal.hh"
#include "StdoutEmitter.hh"

#include "../config.h"

#ifdef ENABLE_GTK
#include "GtkEmitter.hh"
#endif //ENABLE_GTK

#include <iostream>
#include <cstdlib>
#include <getopt.h>

namespace gPWS {

using namespace std;

char const *const DEFAULT_FILE = ".gpwsafe.psafe3";

cApp::cApp(char const *program_name)
	: _program_name(program_name)
	, _command(_C_NOP)
	, _emitter(
#ifdef ENABLE_XCLIP
	           _E_XCLIP
#else
	           _E_STDOUT
#endif //ENABLE_XCLIP
               )
	, _user(false)
	, _pass(false)
	, _argument(0)
{
	char const *home = getenv("HOME");
	if (home)
	{
		_file_name = home;
		_file_name += "/";
	}
	_file_name += DEFAULT_FILE;
}

void cApp::_Usage(bool fail)
{
	ostream &os = fail ? cerr : cout;
	if (!fail)
	{
		os << _program_name
		   << " - command line tool compatible with"
		      " Conterpane's PasswordSafe\n";
	}
	os << "Usage: " << _program_name << " [OPTION] command [ARG]\n";
	os << "Options:\n"
	      "  -f, --file=DATABASE_FILE   specify the database file (~/"
	   << DEFAULT_FILE << " by default)\n"
	      "  -u, --user                 emit username of listed account\n"
	      "  -p, --pass                 emit password of listed account\n"
	      "  -E, --echo                 force echoing of entry to stdout\n"
#ifdef ENABLE_XCLIP
	      "  -x, --xclip                force copying of entry to X selection\n"
#endif //ENABLE_XCLIP
	      "  -h, --help                 display this help and exit\n"
	      "  -V, --version              output version information and exit\n"
	      "Commands:\n"
	      "  --create                   create an empty database\n"
	      "  [--list] [REGEX]           list all [matching] entries."
	                                    " If either -u or -p\n"
	      "                             are given, only one entry may match\n"
	      "  -a, --add [NAME]           add an entry\n"
	      "  -e, --edit REGEX           edit an entry\n"
	      ;
	os << flush;
	throw ExitEx(fail ? 1 : 0);
}

void cApp::Init(int argc, char *argv[])
{
	while (true)
	{
		static struct option long_options[] =
		{
			{ "help",       no_argument,        0, 'h' },
			{ "file",       required_argument,  0, 'f' },
			{ "list",       no_argument,        0, 'L' },
			{ "create",     no_argument,        0, 'C' },
			{ "add",        no_argument,        0, 'a' },
			{ "edit",       no_argument,        0, 'e' },
			{ "user",       no_argument,        0, 'u' },
			{ "pass",       no_argument,        0, 'p' },
			{ "echo",       no_argument,        0, 'E' },
#ifdef ENABLE_XCLIP
			{ "xclip",      no_argument,        0, 'x' },
#endif //ENABLE_XCLIP
			{ 0, 0, 0, 0 }
		};
		char const *const short_options =
			"h"   // help
			"V"   // version
			"a"   // add
			"e"   // edit
			"f:"  // file
			"u"   // user
			"p"   // pass
			"E"   // force echo to stdout
#ifdef ENABLE_XCLIP
			"x"   // force copying to xclip
#endif //ENABLE_XCLIP
			"";
		int option_index = 0;
		int c = getopt_long(argc, argv,
		                    short_options,
		                    long_options, &option_index);
		if (-1 == c)
			break;
		switch (c)
		{
		case 'h':
			return _Usage(false);
		case 'V':
			cout << _program_name << " " << VERSION << endl;
			throw ExitEx(0);
		case 'f':
			_file_name = optarg;
			break;
		case 'L':
			if (_command != _C_NOP)
				return _Usage(false);
			_command = _C_LIST;
			break;
		case 'a':
			if (_command != _C_NOP)
				return _Usage(false);
			_command = _C_ADD;
			break;
		case 'e':
			if (_command != _C_NOP)
				return _Usage(false);
			_command = _C_EDIT;
			break;
		case 'C':
			if (_command != _C_NOP)
				return _Usage(false);
			_command = _C_CREATE;
			break;
		case 'u':
			_user = true;
			break;
		case 'p':
			_pass = true;
			break;
		case 'E':
			_emitter = _E_STDOUT;
			break;
#ifdef ENABLE_XCLIP
		case 'x':
			_emitter = _E_XCLIP;
			break;
#endif //ENABLE_XCLIP
		default:
			return _Usage(true);
		};
	}

	if (_command == _C_NOP)
		_command = _C_LIST;

	if ((_command == _C_LIST || _command == _C_ADD || _command == _C_EDIT)
	    && optind != argc)
	{
		_argument = argv[optind++];
	}

	if (optind != argc)
	{
		cerr << "Too many arguments" << endl;
		throw ExitEx(1);
	}
}

void cApp::Run()
{
	try
	{
		_Run();
	}
	catch (std::exception const &e)
	{
		cerr << "Exception: " << e.what() << endl;
		throw ExitEx(1);
	}
}

void cApp::_Run()
{
	switch (_command)
	{
	case _C_LIST:
		return _DoList();
	case _C_CREATE:
		return _DoCreate();
	case _C_ADD:
		return _DoAdd();
	case _C_EDIT:
		return _DoEdit();
	default:
		cerr << "Command " << _command << " isn't implemented" << endl;
		throw ExitEx(1);
	}
}

namespace {

cDatabase::PtrT _OpenDatabase(string const &file_name)
{
	cDatabase::PtrT database(new cDatabase);
	string prompt = "Enter password for " + file_name + ": ";
	StringX password = cTerminal::GetPassword(prompt);
	database->Read(file_name, password);
	return database;
}

} //namespace

void cApp::_PrintIntention(iEmitter const *emitter)
{
	if (!_user && !_pass)
		return;
	string subject;
	if (_user)
		subject += "login";
	if (_pass)
	{
		if (_user)
			subject += " and ";
		subject += "password";
	}
	emitter->PrintIntention(subject);
}

namespace {

template<typename EntriesT>
bool _CheckSingleEntry(EntriesT const &entries)
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

} //namespace;

void cApp::_DoList()
{
	unique_ptr<iEmitter> emitter;
	switch (_emitter)
	{
	case _E_STDOUT:
		emitter.reset(new cStdoutEmitter);
		break;
#ifdef ENABLE_XCLIP
	case _E_XCLIP:
#ifdef ENABLE_GTK
		emitter.reset(new cGtkEmitter);
#endif //ENABLE_GTK
		break;
#endif //ENABLE_XCLIP
	}

	assert(emitter.get() && "Not implemented _emitter");

	_PrintIntention(emitter.get());

	cDatabase::PtrT database = _OpenDatabase(_file_name);

	auto match = database->Find(_argument);
	if (match.empty())
	{
		cerr << "No matching entries found" << endl;
		throw ExitEx(1);
	}

	if (!_user && !_pass)
	{
		for (auto const &title_entry : match)
			cout << title_entry.first << endl;
		return;
	}

	if (!_CheckSingleEntry(match))
		throw ExitEx(1);

	auto const &title_entry = match.front();
	if (_user)
	{
		emitter->Emit("username for " + title_entry.first,
		              title_entry.second->GetUser());
	}
	if (_pass)
	{
		emitter->Emit("password for " + title_entry.first,
		              title_entry.second->GetPass());
	}
}

void cApp::_DoCreate()
{
	if (!::access(_file_name.c_str(), F_OK))
	{
		cerr << _file_name << " already exists" << endl;
		throw ExitEx(1);
	}

	string prompt1 = "Enter passphrase for " + _file_name + ": ";
	StringX pass1 = cTerminal::GetPassword(prompt1);
	string prompt2 = "Reenter passphrase for " + _file_name + ": ";
	StringX pass2 = cTerminal::GetPassword(prompt2);
	if (pass1 != pass2)
	{
		cerr << "Passphrases do not match" << endl;
		throw ExitEx(1);
	}

	cDatabase::PtrT database(new cDatabase);
	database->Create();
	database->Write(_file_name, pass1);
}

void cApp::_DoAdd()
{
	// Copied from pwsafe
	char const *name = _argument;

	cDatabase::PtrT database = _OpenDatabase(_file_name);
	cEntry::PtrT entry(cEntry::Create());

	if (name)
	{
		// group.title may be given via command line
		char const *dot = strchr(name, '.');
		if (dot &&
		    dot != name &&
		    dot[1] &&
		    strrchr(name, '.') == dot)
		{
			entry->SetTitle(dot + 1);
			entry->SetGroup(StringX(name, dot - name));
		}
		else
		{
			entry->SetTitle(name);
		}
	}

    // FIXME: Limit count of retries.
	while (true)
	{
		if (entry->GetTitle().empty())
			entry->SetTitle(cTerminal::GetText("name: "));
		if (entry->GetGroup().empty())
			entry->SetGroup(cTerminal::GetText("group [<none>]: "));

		if (database->HasEntry(entry->GetFullTitle()))
		{
			cerr << entry->GetFullTitle() << " already exists" << endl;
			if (name)
				throw ExitEx(1);
			entry->SetTitle("");
			entry->SetGroup("");
		}
		else if (!entry->GetTitle().empty())
			break;
	}

	entry->SetUser(cTerminal::GetText("username: "));
	if (entry->GetUser().empty())
	{
		// FIXME!!!
		throw ExitEx(1);
		//e.default_login = getyn("use default username ("+e.the_default_login+") ? [n] ", false);
	}

	StringX pass =
		cTerminal::EnterPassword("password [return for random]: ",
		                         "password again: ");
	entry->SetPass(pass);

	entry->SetNotes(cTerminal::GetText("notes: "));

	database->AddEntry(entry);
	database->Write();
}

void cApp::_DoEdit()
{
	char const *query = _argument;
	if (!query)
	{
		cerr << "An entry must be specified" << endl;
		throw ExitEx(1);
	}

	cDatabase::PtrT database = _OpenDatabase(_file_name);
	auto entries = database->Find(query);
	if (entries.empty())
	{
		cerr << "No matching entries found" << endl;
		throw ExitEx(1);
	}
	if (!_CheckSingleEntry(entries))
		throw ExitEx(1);

	// Original entry
	cEntry::PtrT e_orig = entries.front().second;

	// Copied from pwsafe
	cEntry::PtrT e = e_orig->Copy(); // make a local copy to edit

	while (true)
	{
		e->SetTitle(cTerminal::GetText("name: [" + e_orig->GetTitle() + "] ",
		                               e_orig->GetTitle()));
		e->SetGroup(cTerminal::GetText("group: [" + e_orig->GetGroup() + "] ",
		                               e_orig->GetGroup()));
		if ((e->GetTitle() == e_orig->GetTitle() &&
		     e->GetGroup() == e_orig->GetGroup()) ||
		    !database->HasEntry(e->GetFullTitle()))
			// e.name cannot be empty b/c if the user entered an empty string
			// they got the old name
			break;
		cout << e->GetFullTitle() << " already exists" << endl;
	}

	// FIXME!!!: What's the default login?
	//if (e.default_login)
	//    e.default_login = getyn("keep default username ("+e_orig.the_default_login+") ? [y]", true);
	//if (!e.default_login) {
	//    e.login = gettxt("username: ["+e_orig.login+"] ", e_orig.login);
	//    if (e.login.empty() && !e_orig.default_login) // no point in asking if they just disabled default login
	//        e.default_login = getyn("use default username ("+e_orig.the_default_login+") ? [n]", false);
	//}

	while (true)
	{
		if (!cTerminal::GetYN("change password ? [n] ", false))
			break;

		StringX new_pw =
			cTerminal::EnterPassword("new password: [return for random] ",
			                         "new password again: ");
		if (new_pw.empty() && !e->GetPass().empty())
		{
			if (!cTerminal::GetYN("Confirm changing to an empty password?"
			                      " [n] "))
				continue;
		}
		e->SetPass(new_pw);
		break;
	}

	e->SetNotes(cTerminal::GetText("notes: [<keep same>] ",
	                               e_orig->GetNotes()));

	cEntry::DiffT diff = e->Diff(e_orig);
	if (diff.empty())
	{
		cout << "No change" << endl;
		return;
	}

	std::string prompt = "Confirm changing ";
	for (auto i = diff.begin(); i != diff.end(); ++i)
	{
		if (i != diff.begin())
			prompt += ", ";
		switch (i->type)
		{
		case cEntry::FT_GROUP:
			prompt += "group";
			break;
		case cEntry::FT_TITLE:
			prompt += "name";
			break;
		case cEntry::FT_PASS:
			prompt += "password";
			break;
		case cEntry::FT_NOTES:
			prompt += "notes";
			break;
		default:
			assert(!"Implement this");
		}
	}
	prompt += "? [y] ";
	if (!cTerminal::GetYN(prompt, true))
	{
		cout << "Changes abandoned" << endl;
		return;
	}

	database->RemoveEntry(e_orig);
	database->AddEntry(e);
	database->Write();
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
