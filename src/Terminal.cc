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

#include <iostream>
#include <stdexcept>
#include <boost/scope_exit.hpp>
#include <readline/readline.h>

namespace gPWS {

using namespace std;

StringX cTerminal::GetPassword(char const *prompt)
{
    struct termios tio;
    if (tcgetattr(STDIN_FILENO, &tio))
        throw runtime_error("Can't get termios");
    struct termios new_tio = tio;
    new_tio.c_lflag &= ~ECHO;
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_tio))
        throw runtime_error("Can't set termios");

    BOOST_SCOPE_EXIT((&tio)) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &tio);
        cout << endl;
    } BOOST_SCOPE_EXIT_END

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

//////////////////////////////////////////////////////////////////////////////

cRawTerminal::cRawTerminal()
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
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
