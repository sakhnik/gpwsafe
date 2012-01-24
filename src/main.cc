//
// main.cc
//
//     Created: 10.01.2012
//      Author: A. Sakhnik
//

#include "Database.hh"
#include "Terminal.hh"
#include "../config.h"

#include <iostream>
#include <gcrypt.h>
#include <getopt.h>
#include <sys/stat.h>

using namespace std;
using namespace gPWS;

char const *const DEFAULT_FILE = ".gpwsafe.psafe3";
char const *program_name = "gpwsafe";

static int _Usage(bool fail)
{
    ostream &os = fail ? cerr : cout;
    if (!fail)
    {
        os << program_name
           << " - command line tool compatible with"
              " Conterpane's PasswordSafe\n";
    }
    os << "Usage: " << program_name << " [OPTION] command [ARG]\n";
    os << "Options:\n"
          "  -f, --file=DATABASE_FILE   specify the database file (~/"
       << DEFAULT_FILE << " by default)\n"
          "  -u, --user                 emit username of listed account\n"
          "  -p, --password             emit password of listed account\n"
          "  -E, --echo                 force echoing of entry to stdout\n"
          "  -h, --help                 display this help and exit\n"
          "Commands:\n"
          "  -V, --version              output version information and exit\n"
          "  [--list] [REGEX]           list all [matching] entries\n"
          ;
    os << flush;
    return fail ? 1 : 0;
}

static char const *_Basename(char const *path)
{
    char const *res = strrchr(path, '/');
    if (!res)
        return path;
    return res + 1;
}

static cDatabase::PtrT OpenDatabase(string const &file_name)
{
    cDatabase::PtrT database(new cDatabase);
    string prompt = "Enter password for " + file_name + ": ";
    StringX password =
        cTerminal::GetPassword(prompt.c_str());
    database->Read(file_name.c_str(), password.c_str());
    return database;
}

static int DoList(string const &file_name,
                  char const *query)
{
    cDatabase::PtrT database = OpenDatabase(file_name);
    typedef cDatabase::EntriesT EntriesT;
    EntriesT match = database->Find(query);
    if (match.empty())
    {
        cout << "No matching entries found" << endl;
        return 1;
    }
    for (EntriesT::const_iterator i = match.begin();
         i != match.end(); ++i)
    {
        cEntry::PtrT const &entry(*i);
        cout << entry->GetFullTitle() << endl;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    program_name = _Basename(argv[0]);

    // Be nice and paranoid
    umask(0077);

    char const *const REQ_GCRYPT_VERSION = "1.2.0";
    if (!gcry_check_version(REQ_GCRYPT_VERSION))
    {
        cerr << "libgcrypt version mismatch\n" << endl;
        return 1;
    }
    // Allocate secure memory for sensitive information (won't be swapped)
    gcry_control(GCRYCTL_INIT_SECMEM, 16384, 0);
    gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);

    string file_name;
    char const *home = getenv("HOME");
    if (home)
    {
        file_name = home;
        file_name += "/";
    }
    file_name += DEFAULT_FILE;

    enum eCommand
    {
        C_LIST = 0
    } command = C_LIST;

    while (true)
    {
        static struct option long_options[] =
        {
            { "help", no_argument, 0, 'h' },
            { "file", required_argument, 0, 'f' },
            { "list", no_argument, 0, 'L' },
            { "user", no_argument, 0, 'u' },
            { "password", no_argument, 0, 'p' },
            { "echo", no_argument, 0, 'E' },
            { 0, 0, 0, 0 }
        };
        char const *const short_options =
            "h"   // help
            "V"   // version
            "f:"  // file
            "u"   // user
            "p"   // password
            "E"   // force echo to stdout
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
            cout << program_name << " " << VERSION << endl;
            return 0;
        case 'f':
            file_name = optarg;
            break;
        case 'L':
            command = C_LIST;
            break;
        default:
            return _Usage(true);
        };
    }

    char const *argument = NULL;
    if (command == C_LIST && optind != argc)
    {
        argument = argv[optind++];
    }

    if (optind != argc)
    {
        cerr << "Too many arguments" << endl;
        return 1;
    }

    try
    {
        switch (command)
        {
        case C_LIST:
            return DoList(file_name, argument);
        default:
            cerr << "Command " << command << " isn't implemented" << endl;
            return 1;
        }
    }
    catch (std::exception const& e)
    {
        cerr << "Exception: " << e.what() << endl;
        return 1;
    }
    return 0;
}

// vim: set et ts=4 sw=4:
