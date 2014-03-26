//
// main.cc
//
//     Created: 10.01.2012
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

#include "App.hh"
#include "Gcrypt.hh"
#include "Exceptions.hh"
#include "i18n.h"

#include <sys/stat.h>
#include <iostream>
#include <cstring>
#include <clocale>

using namespace std;
using namespace gPWS;

namespace {

char const *_Basename(char const *path)
{
	char const *res = strrchr(path, '/');
	if (!res)
		return path;
	return res + 1;
}

} //namespace;

int main(int argc, char* argv[])
{
	char const *program_name = _Basename(argv[0]);

	// Initialize gettext
	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);

	// Be nice and paranoid
	umask(0077);

	if (int res = cGcrypt::CheckVersion())
	{
		cerr << _("libgcrypt version mismatch") << endl;
		return res;
	}

	try
	{
		cApp app(program_name);
		app.Init(argc, argv);

		if (int res = cGcrypt::Init(app.UseWeakRandomnessForTests()))
		{
			cerr << _("Can't initialize libgcrypt") << endl;
			return res;
		}

		app.Run();
	}
	catch (ExitEx const &e)
	{
		return e.retcode;
	}
	catch (std::exception const &e)
	{
		cerr << e.what() << endl;
		return 1;
	}
	return 0;
}

// vim: set noet ts=4 sw=4 tw=80:
