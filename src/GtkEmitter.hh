//
// GtkEmitter.hh
//
//     Created: 25.01.2012
//      Author: A. Sakhnik
//

#pragma once

#include "IEmitter.hh"

namespace gPWS {

class cGtkEmitter
    : public iEmitter
{
public:
    cGtkEmitter();
    ~cGtkEmitter();

private:
    virtual void PrintIntention(std::string const &subject) const;
    virtual void Emit(StringX const &name, StringX const &val);
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
