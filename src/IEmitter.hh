//
// IEmitter.hh
//
//     Created: 25.01.2012
//      Author: A. Sakhnik
//

#pragma once

#include "Defs.hh"

#include <boost/noncopyable.hpp>

namespace gPWS {

struct iEmitter
    : boost::noncopyable
{
    virtual ~iEmitter() { }

    virtual char const *GetAction() const = 0;

    virtual void Emit(StringX const &name, StringX const &val) = 0;
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
