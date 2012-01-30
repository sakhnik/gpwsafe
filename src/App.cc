//
// App.cc
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

#include "App.hh"
#include "Database.hh"
#include "Terminal.hh"
#include "StdoutEmitter.hh"

#include "../config.h"

#ifdef ENABLE_GTK
#include "GtkEmitter.hh"
#endif //ENABLE_GTK

#include <iostream>
#include <cstdlib>
#include <getopt.h>

namespace gPWS {

using namespace std;

char const *const DEFAULT_FILE = ".gpwsafe.psafe3";

cApp::cApp(char const *program_name)
    : _program_name(program_name)
    , _command(_C_LIST)
    , _emitter(
#ifdef ENABLE_XCLIP
               _E_XCLIP
#else
               _E_STDOUT
#endif //ENABLE_XCLIP
               )
    , _user(false)
    , _pass(false)
    , _argument(0)
{
    char const *home = getenv("HOME");
    if (home)
    {
        _file_name = home;
        _file_name += "/";
    }
    _file_name += DEFAULT_FILE;
}

void cApp::_Usage(bool fail)
{
    ostream &os = fail ? cerr : cout;
    if (!fail)
    {
        os << _program_name
           << " - command line tool compatible with"
              " Conterpane's PasswordSafe\n";
    }
    os << "Usage: " << _program_name << " [OPTION] command [ARG]\n";
    os << "Options:\n"
          "  -f, --file=DATABASE_FILE   specify the database file (~/"
       << DEFAULT_FILE << " by default)\n"
          "  -u, --user                 emit username of listed account\n"
          "  -p, --pass                 emit password of listed account\n"
          "  -E, --echo                 force echoing of entry to stdout\n"
#ifdef ENABLE_XCLIP
          "  -x, --xclip                force copying of entry to X selection\n"
#endif //ENABLE_XCLIP
          "  -h, --help                 display this help and exit\n"
          "  -V, --version              output version information and exit\n"
          "Commands:\n"
          "  --create                   create an empty database\n"
          "  [--list] [REGEX]           list all [matching] entries\n"
          ;
    os << flush;
    throw ExitEx(fail ? 1 : 0);
}

void cApp::Init(int argc, char *argv[])
{
    while (true)
    {
        static struct option long_options[] =
        {
            { "help",       no_argument,        0, 'h' },
            { "file",       required_argument,  0, 'f' },
            { "list",       no_argument,        0, 'L' },
            { "create",     no_argument,        0, 'C' },
            { "user",       no_argument,        0, 'u' },
            { "pass",       no_argument,        0, 'p' },
            { "echo",       no_argument,        0, 'E' },
#ifdef ENABLE_XCLIP
            { "xclip",      no_argument,        0, 'x' },
#endif //ENABLE_XCLIP
            { 0, 0, 0, 0 }
        };
        char const *const short_options =
            "h"   // help
            "V"   // version
            "f:"  // file
            "u"   // user
            "p"   // pass
            "E"   // force echo to stdout
#ifdef ENABLE_XCLIP
            "x"   // force copying to xclip
#endif //ENABLE_XCLIP
            "";
        int option_index = 0;
        int c = getopt_long(argc, argv,
                            short_options,
                            long_options, &option_index);
        if (-1 == c)
            break;
        switch (c)
        {
        case 'h':
            return _Usage(false);
        case 'V':
            cout << _program_name << " " << VERSION << endl;
            throw ExitEx(0);
        case 'f':
            _file_name = optarg;
            break;
        case 'L':
            _command = _C_LIST;
            break;
        case 'C':
            _command = _C_CREATE;
            break;
        case 'u':
            _user = true;
            break;
        case 'p':
            _pass = true;
            break;
        case 'E':
            _emitter = _E_STDOUT;
            break;
#ifdef ENABLE_XCLIP
        case 'x':
            _emitter = _E_XCLIP;
            break;
#endif //ENABLE_XCLIP
        default:
            return _Usage(true);
        };
    }

    if (_command == _C_LIST && optind != argc)
    {
        _argument = argv[optind++];
    }

    if (optind != argc)
    {
        cerr << "Too many arguments" << endl;
        throw ExitEx(1);
    }
}

void cApp::Run()
{
    try
    {
        _Run();
    }
    catch (std::exception const &e)
    {
        cerr << "Exception: " << e.what() << endl;
        throw ExitEx(1);
    }
}

void cApp::_Run()
{
    switch (_command)
    {
    case _C_LIST:
        return _DoList();
    case _C_CREATE:
        return _DoCreate();
    default:
        cerr << "Command " << _command << " isn't implemented" << endl;
        throw ExitEx(1);
    }
}

static cDatabase::PtrT _OpenDatabase(string const &file_name)
{
    cDatabase::PtrT database(new cDatabase);
    string prompt = "Enter password for " + file_name + ": ";
    StringX password =
        cTerminal::GetPassword(prompt.c_str());
    database->Read(file_name.c_str(), password.c_str());
    return database;
}

void cApp::_PrintIntention(iEmitter const *emitter)
{
    if (!_user && !_pass)
        return;
    string subject;
    if (_user)
        subject += "login";
    if (_pass)
    {
        if (_user)
            subject += " and ";
        subject += "password";
    }
    emitter->PrintIntention(subject);
}

void cApp::_DoList()
{
    auto_ptr<iEmitter> emitter;
    switch (_emitter)
    {
    case _E_STDOUT:
        emitter.reset(new cStdoutEmitter);
        break;
#ifdef ENABLE_XCLIP
    case _E_XCLIP:
#ifdef ENABLE_GTK
        emitter.reset(new cGtkEmitter);
#endif //ENABLE_GTK
        break;
#endif //ENABLE_XCLIP
    }

    assert(emitter.get() && "Not implemented _emitter");

    _PrintIntention(emitter.get());

    cDatabase::PtrT database = _OpenDatabase(_file_name);

    typedef cDatabase::EntriesT EntriesT;
    EntriesT match = database->Find(_argument);
    if (match.empty())
    {
        cout << "No matching entries found" << endl;
        throw ExitEx(1);
    }

    if (!_user && !_pass)
    {
        for (EntriesT::const_iterator i = match.begin();
             i != match.end(); ++i)
        {
            cEntry::PtrT const &entry(*i);
            cout << entry->GetFullTitle() << endl;
        }
        return;
    }

    if (match.size() != 1)
    {
        cout << "More than one matching entry: ";
        int j = 0;
        for (EntriesT::const_iterator i = match.begin();
             i != match.end() && j != 3; ++i, ++j)
        {
            if (j)
                cout << ", ";
            cout << (*i)->GetFullTitle();
        }
        int rest = match.size() - j;
        if (rest)
            cout << ", ... (" << rest << " more)";
        cout << " ." << endl;
        throw ExitEx(1);
    }

    cEntry::PtrT const &entry = match.front();
    if (_user)
    {
        emitter->Emit("username for " + entry->GetFullTitle(),
                      entry->GetUser());
    }
    if (_pass)
    {
        emitter->Emit("password for " + entry->GetFullTitle(),
                      entry->GetPass());
    }
}

void cApp::_DoCreate()
{
    if (!::access(_file_name.c_str(), F_OK))
    {
        cerr << _file_name << " already exists" << endl;
        throw ExitEx(1);
    }

    string prompt1 = "Enter passphrase for " + _file_name + ":";
    StringX pass1 = cTerminal::GetPassword(prompt1.c_str());
    string prompt2 = "Reenter passphrase for " + _file_name + ":";
    StringX pass2 = cTerminal::GetPassword(prompt2.c_str());
    if (pass1 != pass2)
    {
        cerr << "Passphrases do not match" << endl;
        throw ExitEx(1);
    }

    cDatabase db;
    db.Create();
    db.Write(_file_name.c_str(), pass1.c_str());
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
