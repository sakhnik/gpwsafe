//
// Command.cc
//
//     Created: 26.10.2013
//      Author: A. Sakhnik
//

#include "Command.hh"

namespace gPWS {

cCommand::Params::Params()
	: user(false)
	, pass(false)
{
	char const *const DEFAULT_FILE = ".gpwsafe.psafe3";
	char const *home = getenv("HOME");
	if (home)
	{
		file_name = home;
		file_name += "/";
	}
	file_name += DEFAULT_FILE;
}

} //namespace gPWS;

// vim: set noet ts=4 sw=4 tw=80:
