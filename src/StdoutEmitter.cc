//
// StdoutEmitter.cc
//
//     Created: 25.01.2012
//      Author: A. Sakhnik
//

#include "StdoutEmitter.hh"

namespace gPWS {

using namespace std;

void cStdoutEmitter::Emit(StringX const &name, StringX const &val)
{
    cout << name << ": " << val << endl;
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
