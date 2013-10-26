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

namespace gPWS {

using namespace std;

cCommand::PtrT cCommandList::Create(string const &regex)
{
	return cCommand::PtrT(new cCommandList(regex));
}

cCommandList::cCommandList(string const &regex)
	: _regex(regex)
{
}

void cCommandList::Execute(Params const &params)
{
//	unique_ptr<iEmitter> emitter;
//	switch (_emitter)
//	{
//	case _E_STDOUT:
//		emitter.reset(new cStdoutEmitter);
//		break;
//#ifdef ENABLE_XCLIP
//	case _E_XCLIP:
//#ifdef ENABLE_GTK
//		emitter.reset(new cGtkEmitter);
//#endif //ENABLE_GTK
//		break;
//#endif //ENABLE_XCLIP
//	}
//
//	assert(emitter.get() && "Not implemented _emitter");
//
//	_PrintIntention(emitter.get());
//
//	cDatabase::PtrT database = _OpenDatabase(_file_name);
//
//	auto match = database->Find(_argument);
//	if (match.empty())
//	{
//		cerr << "No matching entries found" << endl;
//		throw ExitEx(1);
//	}
//
//	if (!_user && !_pass)
//	{
//		for (auto const &title_entry : match)
//			cout << title_entry.first << endl;
//		return;
//	}
//
//	if (!_CheckSingleEntry(match))
//		throw ExitEx(1);
//
//	auto const &title_entry = match.front();
//	if (_user)
//	{
//		emitter->Emit("username for " + title_entry.first,
//		              title_entry.second->GetUser());
//	}
//	if (_pass)
//	{
//		emitter->Emit("password for " + title_entry.first,
//		              title_entry.second->GetPass());
//	}
}

void cCommandList::_PrintIntention(Params const &params)
{
	if (!params.user && !params.pass)
		return;
	string subject;
	if (params.user)
		subject += "login";
	if (params.pass)
	{
		if (params.user)
			subject += " and ";
		subject += "password";
	}
	params.emitter->PrintIntention(subject);
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
