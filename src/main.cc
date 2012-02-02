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

#include "../config.h"
#include "App.hh"
#include "Gcrypt.hh"
#include "Privileges.hh"

#include <sys/stat.h>
#include <iostream>
#include <cstring>

using namespace std;
using namespace gPWS;

static char const *_Basename(char const *path)
{
    char const *res = strrchr(path, '/');
    if (!res)
        return path;
    return res + 1;
}

int main(int argc, char* argv[])
{
    char const *program_name = _Basename(argv[0]);

    // Drop privileges if setuid root
    cPrivileges::Init();

    // Be nice and paranoid
    umask(0077);

    if (int res = cGcrypt::Init())
    {
        cerr << "Can't initialize libgcrypt" << endl;
        return res;
    }

    try
    {
        cApp app(program_name);
        app.Init(argc, argv);

        app.Run();
    }
    catch (cApp::ExitEx const &e)
    {
        return e.retcode;
    }
    return 0;
}

// vim: set et ts=4 sw=4:
