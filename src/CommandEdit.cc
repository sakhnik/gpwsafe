//
// CommandEdit.cc
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

#include "CommandEdit.hh"
#include "Exceptions.hh"
#include "Terminal.hh"

namespace gPWS {

using namespace std;

cCommand::PtrT cCommandEdit::Create(string const &regex)
{
	return cCommand::PtrT(new cCommandEdit(regex));
}

cCommandEdit::cCommandEdit(string const &regex)
	: _regex(regex)
{
}

void cCommandEdit::Execute(Params const &params)
{
	if (_regex.empty())
	{
		cerr << "An entry must be specified" << endl;
		throw ExitEx(1);
	}
	char const *query = _regex.c_str();

	cDatabase::PtrT database = OpenDatabase(params.ExpandFileName());
	auto entries = database->Find(query);
	if (entries.empty())
	{
		cerr << "No matching entries found" << endl;
		throw ExitEx(1);
	}
	if (!CheckSingleEntry(entries))
		throw ExitEx(1);

	// Original entry
	cEntry::PtrT e_orig = entries.front()->second;

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
