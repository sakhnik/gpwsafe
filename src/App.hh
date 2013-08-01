//
// App.hh
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

#pragma once

#include "../config.h"

#include <string>
#include <boost/noncopyable.hpp>

namespace gPWS {

struct iEmitter;

class cApp
	: boost::noncopyable
{
public:
	cApp(char const *program_name);

	void Init(int argc, char *argv[]);

	void Run();

	struct ExitEx
	{
		int retcode;
		ExitEx(int retcode_) : retcode(retcode_) { }
	};

private:
	char const *_program_name;
	std::string _file_name;

	enum eCommand
	{
		_C_NOP = 0,
		_C_LIST,
		_C_CREATE,
		_C_ADD,
		_C_EDIT
	} _command;

	enum eEmitter
	{
#ifdef ENABLE_XCLIP
		_E_XCLIP,
#endif //ENABLE_XCLIP
		_E_STDOUT
	} _emitter;

	bool _user;
	bool _pass;

	char const *_argument;

	void _Run();
	void _Usage(bool fail);
	void _DoList();
	void _PrintIntention(iEmitter const *emitter);
	void _DoCreate();
	void _DoAdd();
	void _DoEdit();
};

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
