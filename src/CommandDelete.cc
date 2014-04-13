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

cCommand::PtrT cCommandDelete::Create(const string &regex)
{
	return cCommand::PtrT(new cCommandDelete(regex));
}

cCommandDelete::cCommandDelete(const string &regex)
	: _regex(regex)
{
}

void cCommandDelete::Execute(const Params &params)
{
	if (_regex.empty())
	{
		cerr << _("An entry must be specified") << endl;
		throw ExitEx(1);
	}
	char const *query = _regex.c_str();

	cDatabase::PtrT database = OpenDatabase(params.ExpandFileName());
	auto entries = database->Find(query);
	if (entries.empty())
	{
		cerr << _("No matching entries found") << endl;
		throw ExitEx(1);
	}
	if (!CheckSingleEntry(entries))
		throw ExitEx(1);

	// Original entry
	cEntry::PtrT e = entries.front()->second;

	auto confirm = _("Confirm deleting ") + e->GetFullTitle() + " ? [n] ";
	if (!cTerminal::GetYN(confirm.c_str(), false))
		return;

	database->RemoveEntry(e);
	database->Write();
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
