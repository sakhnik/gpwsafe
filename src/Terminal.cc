//
// Terminal.cc
//
//     Created: 22.01.2012
//      Author: A. Sakhnik
//

#include "Terminal.hh"

#include <termios.h>
#include <readline/readline.h>
#include <stdexcept>
#include <boost/scope_exit.hpp>

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

} //namespace gPWS;

// vim: set et ts=4 sw=4:
