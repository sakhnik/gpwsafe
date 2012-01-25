//
// App.cc
//
//     Created: 24.01.2012
//      Author: A. Sakhnik
//

#include "App.hh"
#include "Database.hh"
#include "Terminal.hh"
#include "StdoutEmitter.hh"
#include "GtkEmitter.hh"
#include "../config.h"

#include <iostream>
#include <cstdlib>
#include <getopt.h>

namespace gPWS {

using namespace std;

char const *const DEFAULT_FILE = ".gpwsafe.psafe3";

cApp::cApp(char const *program_name)
    : _program_name(program_name)
    , _command(_C_LIST)
    , _emitter(_E_XCLIP)
    , _user(false)
    , _password(false)
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

int cApp::_Usage(bool fail)
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
          "  -p, --password             emit password of listed account\n"
          "  -E, --echo                 force echoing of entry to stdout\n"
          "  -x, --xclip                force copying of entry to X selection\n"
          "  -h, --help                 display this help and exit\n"
          "Commands:\n"
          "  -V, --version              output version information and exit\n"
          "  [--list] [REGEX]           list all [matching] entries\n"
          ;
    os << flush;
    return fail ? 1 : 0;
}

int cApp::Init(int argc, char *argv[])
{
    while (true)
    {
        static struct option long_options[] =
        {
            { "help",       no_argument,        0, 'h' },
            { "file",       required_argument,  0, 'f' },
            { "list",       no_argument,        0, 'L' },
            { "user",       no_argument,        0, 'u' },
            { "password",   no_argument,        0, 'p' },
            { "echo",       no_argument,        0, 'E' },
            { "xclip",      no_argument,        0, 'x' },
            { 0, 0, 0, 0 }
        };
        char const *const short_options =
            "h"   // help
            "V"   // version
            "f:"  // file
            "u"   // user
            "p"   // password
            "E"   // force echo to stdout
            "x"   // force copying to xclip
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
            return 0;
        case 'f':
            _file_name = optarg;
            break;
        case 'L':
            _command = _C_LIST;
            break;
        case 'u':
            _user = true;
            break;
        case 'p':
            _password = true;
            break;
        case 'E':
            _emitter = _E_STDOUT;
            break;
        case 'x':
            _emitter = _E_XCLIP;
            break;
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
        return 1;
    }

    return 0;
}

int cApp::Run()
{
    try
    {
        return _Run();
    }
    catch (std::exception const &e)
    {
        cerr << "Exception: " << e.what() << endl;
        return 1;
    }
}

int cApp::_Run()
{
    switch (_command)
    {
    case _C_LIST:
        return _DoList();
    default:
        cerr << "Command " << _command << " isn't implemented" << endl;
        return 1;
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
    if (!_user && !_password)
        return;
    string subject;
    if (_user)
        subject += "login";
    if (_password)
    {
        if (_user)
            subject += " and ";
        subject += "password";
    }
    emitter->PrintIntention(subject);
}

int cApp::_DoList()
{
    auto_ptr<iEmitter> emitter;
    switch (_emitter)
    {
    case _E_STDOUT:
        emitter.reset(new cStdoutEmitter);
        break;
    case _E_XCLIP:
        emitter.reset(new cGtkEmitter);
        break;
    default:
        assert(!"Not implemented _emitter");
    }

    _PrintIntention(emitter.get());

    cDatabase::PtrT database = _OpenDatabase(_file_name);
    typedef cDatabase::EntriesT EntriesT;
    EntriesT match = database->Find(_argument);
    if (match.empty())
    {
        cout << "No matching entries found" << endl;
        return 1;
    }

    if (!_user && !_password)
    {
        for (EntriesT::const_iterator i = match.begin();
             i != match.end(); ++i)
        {
            cEntry::PtrT const &entry(*i);
            cout << entry->GetFullTitle() << endl;
        }
        return 0;
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
    }

    cEntry::PtrT const &entry = match.front();
    if (_user)
    {
        emitter->Emit("username for " + entry->GetFullTitle(),
                      entry->GetUser());
    }
    if (_password)
    {
        emitter->Emit("password for " + entry->GetFullTitle(),
                      entry->GetPass());
    }

    return 0;
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
