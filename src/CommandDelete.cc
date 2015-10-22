//
// CommandDelete.cc
//
//     Created: 13.04.2014
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

#include "CommandDelete.hh"
#include "Exceptions.hh"
#include "Terminal.hh"
#include "i18n.h"

namespace gPWS {

using namespace std;

Command::PtrT CommandDelete::Create()
{
	return Command::PtrT(new CommandDelete);
}

void CommandDelete::SetArgument(const std::string &arg)
{
	_regex = arg;
}

void CommandDelete::Execute(const Params &params)
{
	Database::PtrT database = OpenDatabase(params.ExpandFileName());
	auto entries = database->Find(_regex.c_str());
	if (entries.empty())
	{
		cerr << _("No matching entries found") << endl;
		throw ExitEx(1);
	}

	// If no query was given on the command line, pick up record interactively.
	if (_regex.empty())
	{
		auto idx = Terminal::PickUp(entries.size(),
		                            [&](size_t i) -> const StringX &
		                            {
		                                return entries[i]->first;
		                            });
		if (idx > entries.size())
			throw ExitEx(1);

		const auto &query = entries[idx]->first;
		entries = database->Find(query.c_str());
	}

	if (!CheckSingleEntry(entries))
		throw ExitEx(1);

	// Original entry
	Entry::PtrT e = entries.front()->second;

	auto confirm = _("Confirm deleting ") + e->GetFullTitle() + " ? [n] ";
	if (!Terminal::GetYN(confirm.c_str(), false))
		return;

	database->RemoveEntry(e);
	database->Write();
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
