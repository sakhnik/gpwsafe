//
// Command.hh
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

#pragma once

#include "IEmitter.hh"
#include "Database.hh"

#include <string>
#include <boost/noncopyable.hpp>
#include <memory>

namespace gPWS {

class cCommand
	: boost::noncopyable
{
public:
	typedef std::unique_ptr<cCommand> PtrT;

	struct Params
	{
		std::string file_name;
		bool user;
		bool pass;
		bool notes;
		std::unique_ptr<iEmitter> emitter;

		Params();
		std::string ExpandFileName() const;
	};

	virtual ~cCommand() { }
	virtual void Execute(Params const &params) = 0;

protected:
	static cDatabase::PtrT OpenDatabase(std::string const &file_name);
	static bool CheckSingleEntry(cDatabase::FilterRangeT const &entries);
};

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
