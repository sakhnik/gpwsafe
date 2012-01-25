//
// StdoutEmitter.hh
//
//     Created: 25.01.2012
//      Author: A. Sakhnik
//

#pragma once

#include "IEmitter.hh"

namespace gPWS {

class cStdoutEmitter
    : public iEmitter
{
private:
    virtual char const *GetAction() const
    {
        return "to print";
    }

    virtual void Emit(StringX const &name, StringX const &val);
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
