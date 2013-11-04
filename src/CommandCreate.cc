//
// CommandCreate.cc
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


#include "CommandCreate.hh"
#include "Exceptions.hh"
#include "Terminal.hh"

namespace gPWS {

using namespace std;

cCommand::PtrT cCommandCreate::Create()
{
	return cCommand::PtrT(new cCommandCreate());
}

cCommandCreate::cCommandCreate()
{
}

void cCommandCreate::Execute(Params const &params)
{
	string file_name = params.ExpandFileName();

	if (!::access(file_name.c_str(), F_OK))
	{
		cerr << file_name << " already exists" << endl;
		throw ExitEx(1);
	}

	string prompt1 = "Enter passphrase for " + file_name + ": ";
	StringX pass1 = cTerminal::GetPassword(prompt1);
	string prompt2 = "Reenter passphrase for " + file_name + ": ";
	StringX pass2 = cTerminal::GetPassword(prompt2);
	if (pass1 != pass2)
	{
		cerr << "Passphrases do not match" << endl;
		throw ExitEx(1);
	}

	cDatabase::PtrT database(new cDatabase);
	database->Create();
	database->Write(file_name, pass1);
}

} //namespace gPWS;
// vim: set noet ts=4 sw=4 tw=80:
