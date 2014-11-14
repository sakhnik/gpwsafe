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

#include "Command.hh"

#include <string>
#include <boost/noncopyable.hpp>

namespace gPWS {

struct iEmitter;

class App
	: boost::noncopyable
{
public:
	App(char const *program_name);

	void Init(int argc, char *argv[]);

	bool UseWeakRandomnessForTests() const
		{ return _use_weak_randomness_for_tests; }

	void Run();

private:
	char const *_program_name;
	Command::PtrT _command;
	Command::Params _params;
	bool _use_weak_randomness_for_tests;

	void _SetCommand(Command::PtrT command);
	void _SetEmitter(iEmitter *emitter);
};

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
