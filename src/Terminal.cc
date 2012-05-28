//
// Terminal.cc
//
//     Created: 22.01.2012
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

#include "Terminal.hh"
#include "Random.hh"

#include <iostream>
#include <sstream>
#include <cstdio>
#include <stdexcept>
#include <boost/scope_exit.hpp>
#include <readline/readline.h>

namespace gPWS {

using namespace std;

cRawTerminal::cRawTerminal(bool new_line)
    : _new_line(new_line)
{
    tcgetattr(STDIN_FILENO, &_tio);

    struct termios new_tio = _tio;
    new_tio.c_lflag &= ~(ICANON);
    // A hack for sparc (from pwsafe)
    new_tio.c_cc[VMIN] = 1;
    new_tio.c_cc[VTIME] = 0;
    // Turn off echo too; no need to show them the char they pressed
    new_tio.c_lflag &= ~(ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

cRawTerminal::~cRawTerminal()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &_tio);
    if (_new_line)
        cout << endl;
}

//////////////////////////////////////////////////////////////////////////////

StringX cTerminal::GetPassword(char const *prompt)
{
    cRawTerminal raw_terminal(true);

    rl_completion_entry_function = NULL;
    char *input = readline(prompt);
    if (!input)
        throw runtime_error("Can't get password");
    BOOST_SCOPE_EXIT((&input)) {
        memset(input, 0, strlen(input));
        delete input;
    } BOOST_SCOPE_EXIT_END

    return StringX(input);
}

StringX cTerminal::GetText(char const *prompt,
                           StringX const &def)
{
    rl_completion_entry_function = NULL;
    char *input = readline(prompt);
    if (!input)
        throw runtime_error("Can't get text");
    BOOST_SCOPE_EXIT((&input)) {
        memset(input, 0, strlen(input));
        delete input;
    } BOOST_SCOPE_EXIT_END

    StringX res(input);
    if (res.empty())
        return def;
    return res;
}

StringX cTerminal::EnterPassword(char const *prompt1, char const *prompt2)
{
    // Copied from pwsafe
    while (true)
    {
        StringX pw1 = GetPassword(prompt1);
        if (pw1.empty())
        {
            if (GetYN("Generate random password? [y] ", true))
            {
                pw1 = RandomPassword();
                if (!pw1.empty())
                    return pw1;
                else
                    continue; // back to entering by hand for them
                              // (perhaps they want to copy only a subset
                              //  of the original pw)
            } // else let them have an empty password, though they'll have
              // to enter it twice
        }
        StringX pw2 = GetPassword(prompt2);
        if (pw1 == pw2)
        {
            return pw1;
        }
        cerr << "Passwords do not match" << endl;
    }
}

bool cTerminal::GetYN(char const *prompt, int def_val)
{
    // Copied from pwsafe
    while (true)
    {
        char c = GetChar(prompt, def_val > 0 ? 'y' : def_val == 0 ? 'n' : -1);
        switch (tolower(c))
        {
        case 'y':
            return true;
        case 'n':
            return false;
        // default: prompt again until we get a good answer
        }
    }
}

char cTerminal::GetChar(char const *prompt, const int def_val)
{
    // Copied from pwsafe
    struct termios tio;
    tcgetattr(STDIN_FILENO, &tio);
    {
        termios new_tio = tio;
        new_tio.c_lflag &= ~(ICANON);
        // now that we turn ICANON off we *must* set VMIN=1
        // or on sparc the read() buffers 4 at a time
        new_tio.c_cc[VMIN] = 1;
        new_tio.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    }
    BOOST_SCOPE_EXIT((&tio)){
        tcsetattr(STDIN_FILENO, TCSANOW, &tio);
    } BOOST_SCOPE_EXIT_END

    while (true)
    {
        cout << prompt << flush;
        char x;
        ssize_t rc = read(STDIN_FILENO, &x, 1);

        if (rc != 1)
            throw runtime_error("Read error");
        switch (x)
        {
        case '\r':
            cout << endl;
            // fall through to '\n'
        case '\n':
            if (def_val >= 0)
                return def_val;
            // else there is no default and the user must hit a proper char
            break;
        default:
            cout << endl;
            return x;
        }
        // if we get this far the user didn't answer, and we loop
        // and reprompt them
    }
}

StringX cTerminal::RandomPassword()
{
    // Copied from pwsafe

    // here I implement the 'easyvision' mode of pwsafe 1.9.x
    // where the resulting ascii has nice legibility properties
    // for those who copy these by hand
    const static char all_alphanum[] = "abcdefghijklmnopqrstuvwxyz"
                                       "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                       "0123456789";
    const static char easyvision_alphanum[] = "abcdefghijkmnopqrstuvwxyz"
                                              "ABCDEFGHJKLMNPQRTUVWXY"
                                              "346789";
    const static char easyvision_symbol[] = "+-=_@#$%^&<>/~\\?";
    const static char hex_only[] = "0123456789abcdef";
    const static char digits_only[] = "0123456789";


    int entropy_needed = 20*8; // enough for proper initialization
                               // of a SHA1 hash, and enough for proper direct
                               // keying of 128-bit block ciphers
    int type = 0;
    while (true)
    {
        const char* type_name = "";
        const char* sets[2] = { "", "" };
        int entropy_per_char;
        bool one_char_from_each_type = true;
        switch (type)
        {
        case 0:
            type_name = "alpha/digit/symbol";
            sets[0] = all_alphanum;
            sets[1] = easyvision_symbol;
            entropy_per_char = 628;
            // 100 * log2(26+26+10+16) = log2(78); best case
            break;
        case 1:
            type_name = "alpha/digit";
            sets[0] = all_alphanum;
            entropy_per_char = 595;
            break;
        case 2:
            type_name = "easy-to-read alpha/digit";
            sets[0] = easyvision_alphanum;
            entropy_per_char = 555;
            // 100 * log2(25+22+10) = log2(57); worse case
            break;
        case 3:
            type_name = "easy-to-read alpha/digit/symbol";
            sets[0] = easyvision_alphanum;
            sets[1] = easyvision_symbol;
            entropy_per_char = 597;
            break;
        case 4:
            type_name = "digits only";
            sets[0] = digits_only;
            entropy_per_char = 332; // 100 * log2(10)
            one_char_from_each_type = false;
            break;
        case 5:
            type_name = "hex digits only";
            sets[0] = hex_only;
            entropy_per_char = 400; // 100 * log2(16)
            one_char_from_each_type = false;
            break;
        default:
            // wrap around back to type 0
            type = 0;
            continue;
        }

        const int set0_chars = strlen(sets[0]);
        const int total_chars = set0_chars + strlen(sets[1]);

        // But we are not going to generate all possible passwords
        // because we are going to exclude those that don't have at least
        // one char from each type, so that reduces the entropy_per_char
        // if originally we had 2^(num_chars * entropy_per_char) possible
        // passwords, and we exclude (in the worst case) (and double-counting
        // those passwords that have two types of char missing)
        // (57-25)/57 ^ num_chars + (57-22)/57 ^ num_chars +
        // + (57-10)/57 ^ num_chars of these, we reduce the bits of entropy
        // per char by
        // log2(57)-log2(57-25) + log2(57)-log2(57-22) + log2(57)-log2(57-10) =
        // = 1.82 pessimist bits/char
        if (one_char_from_each_type)
            entropy_per_char -= 182;

        const int num_chars = 1+100*entropy_needed/entropy_per_char;
        // we want 20*8 bits of entropy in our password (thus good enough
        // to create good SHA1 hashes/to key 128-bit key secret key algo's);
        // +1 is in lou of rounding the division properly

        StringX pw;
        bool got_upper, got_lower, got_num, got_sym;
        do
        {
            pw.erase();

            got_upper = false;
            got_lower = false;
            got_num = false;
            got_sym = false;

            for (int i=0; i<num_chars; i++)
            {
                unsigned char idx;
                do
                {
                    cRandom::Randomize(&idx, 1, true);
                    idx &= 0x7f;
                    // might as well strip off the upper bit since
                    // total_chars is never more than 64, and
                    // such a stripping doesn't change the distribution
                } while (idx >= total_chars);

                char c;
                if (idx < set0_chars)
                    c = sets[0][idx];
                else
                    c = sets[1][idx-set0_chars];

                pw += c;

                if (islower(c)) got_lower = true;
                else if (isupper(c)) got_upper = true;
                else if (isdigit(c)) got_num = true;
                else got_sym = true;
            }
        } while (one_char_from_each_type &&
                 (!got_lower ||
                  !got_upper ||
                  !got_num   ||
                  (sets[1][0] && !got_sym)));
        // some systems want one of each type of char in the password,
        // so might as well do it all the time, even though it is a tiny bit
        // less random this way (but we already took that into account
        // in entropy_per_char)

        // see what the user thinks of this one
        OStringStreamX prompt_oss;
        prompt_oss << "Use " << pw << "\n"
                      "type " << type_name << ", "
                      "length " << pw.length() << ", "
                   << entropy_needed
                   << " bits of entropy [y/N/ /+/-/q/?] ? ";
        StringX prompt = prompt_oss.str();
        switch (tolower(GetChar(prompt, 'n')))
        {
        case 'y':
            return pw;
        case 'q':
            return "";
        case ' ':
            type++;
            break;
        case '-':
            if (entropy_needed > 128)
                entropy_needed -= 32;
            else if (entropy_needed > 64)
                entropy_needed -= 16;
            else if (entropy_needed > 32)
                entropy_needed -= 8;
            // else you can't go any lower
            break;
        case '+': case '=':
            if (entropy_needed < 64)
                entropy_needed += 8;
            else if (entropy_needed < 128)
                entropy_needed += 16;
                // so we can hit 112, the magic number for WEP keys
            else
                entropy_needed += 32;
            break;
        case '?': case 'h':
            cout <<
                "Commands:\n"
                "  Y      Yes, accept this password\n"
                "  N      No, generate another password of same type\n"
                "  <space> Cycle through password types\n"
                "  -      Lower the entropy & password length\n"
                "  +      Raise the entropy & password length\n"
                "  Q      Quit\n"
                "  ?      Help\n"
                << endl;
            continue;
            // default: show another password
        }
    }
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
