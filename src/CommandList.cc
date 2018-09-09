//
// CommandList.cc
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

#include "CommandList.hh"
#include "Database.hh"
#include "Terminal.hh"
#include "Exceptions.hh"
#include "Entry.hh"
#include "i18n.h"

#include <algorithm>
#include <iomanip>

namespace gPWS {

using namespace std;

Command::PtrT CommandList::Create()
{
	return Command::PtrT(new CommandList);
}

void CommandList::SetArgument(const std::string &arg)
{
	_regex = arg;
}

void CommandList::Execute(Params const &params)
{
	assert(params.emitter && "Not implemented _emitter");
	auto &emitter = params.emitter;
	emitter->SetSelection(params.selection);

	_PrintIntention(params);

	Database::PtrT database = OpenDatabase(params.ExpandFileName());

	auto match = database->Find(_regex.c_str());
	if (match.empty())
	{
		cerr << _("No matching entries found") << endl;
		throw ExitEx(1);
	}

	if (!params.user && !params.pass && !params.notes)
	{
		if (match.size() > 1)
		{
			if (Terminal::IsOutput())
			{
				auto max_entry =
					max_element(begin(match), end(match),
					            [](const auto &a, const auto &b)
					            {
					                return a->first.size() < b->first.size();
					            });
				auto max_width = (*max_entry)->first.size() + 4;
				auto columns = Terminal::GetColumns() / max_width;

				size_t col{0};
				for (const auto &title_entry : match)
				{
					cout << title_entry->first;
					cout << setw(max_width - title_entry->first.size())
						<< ' ';
					if (++col == columns)
					{
						cout << '\n';
						col = 0;
					}
				}
				if (col)
					cout << '\n';
				cout << flush;
			}
			else
			{
				for (auto const &title_entry : match)
					cout << title_entry->first << endl;
			}
		}
		else
		{
			auto entry = match.front()->second;
			entry->PrettyPrint();
		}
		return;
	}

	// If no query was given on the command line, pick up record interactively.
	if (_regex.empty())
	{
		auto idx = Terminal::PickUp(match.size(),
		                            [&](size_t i) -> const StringX &
		                            {
		                                return match[i]->first;
		                            });
		if (idx > match.size())
			throw ExitEx(1);

		const auto &query = match[idx]->first;
		match = database->Find(query.c_str());
	}

	if (!CheckSingleEntry(match))
		throw ExitEx(1);

	auto const &title_entry = match.front();
	if (params.user)
	{
		emitter->Emit(_("username for ") + title_entry->first,
		              title_entry->second->GetUser());
	}
	if (params.pass)
	{
		emitter->Emit(_("password for ") + title_entry->first,
		              title_entry->second->GetPass());
	}
	if (params.notes)
	{
		emitter->Emit(_("notes for ") + title_entry->first,
		              title_entry->second->GetNotes());
	}

}

void CommandList::_PrintIntention(Params const &params)
{
	if (params.user)
	{
		if (params.pass)
			params.emitter->PrintIntention(_("login and password"));
		else
			params.emitter->PrintIntention(_("login"));
	}
	else
	{
		if (params.pass)
			params.emitter->PrintIntention(_("password"));
		else
			return;
	}
	if (params.notes) 
	{
		params.emitter->PrintIntention(_("notes"));
	}
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
