//
// CommandAdd.cc
//
//     Created: 25.10.2013
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


#include "CommandAdd.hh"
#include "Database.hh"
#include "Terminal.hh"
#include "Exceptions.hh"
#include "i18n.h"

namespace gPWS {

using namespace std;

Command::PtrT CommandAdd::Create()
{
	return Command::PtrT(new CommandAdd);
}

void CommandAdd::SetArgument(const string &arg)
{
	_name = arg;
}

void CommandAdd::Execute(Params const &params)
{
	Database::PtrT database = OpenDatabase(params.ExpandFileName());
	Entry::PtrT entry(Entry::Create());

	if (!_name.empty())
	{
		// group.title may be given via command line
		// FIXME: Rewrite this
		char const *name = _name.c_str();
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
			entry->SetTitle(Terminal::GetText(_("name: ")));
		if (entry->GetGroup().empty())
			entry->SetGroup(Terminal::GetText(_("group [<none>]: ")));

		if (database->HasEntry(entry->GetFullTitle()))
		{
			cerr << entry->GetFullTitle() << _(" already exists") << endl;
			if (!_name.empty())
				throw ExitEx(1);
			entry->SetTitle("");
			entry->SetGroup("");
		}
		else if (!entry->GetTitle().empty())
			break;
	}

	entry->SetUser(Terminal::GetText(_("username: ")));
	if (entry->GetUser().empty())
	{
		// FIXME!!!
		throw ExitEx(1);
		//e.default_login = getyn("use default username ("+e.the_default_login+") ? [n] ", false);
	}

	StringX pass =
		Terminal::EnterPassword(_("password [return for random]: "),
		                        _("password again: "));
	entry->SetPass(pass);

	entry->SetNotes(Terminal::GetText(_("notes: ")));

	database->AddEntry(entry);
	database->Write();
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
