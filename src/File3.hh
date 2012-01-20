//
// File3.hh
//
//     Created: 10.01.2012
//      Author: A. Sakhnik
//

#pragma once

#include "Field.hh"

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>

namespace gPWS {

class cFile3
    : boost::noncopyable
{
public:
    cFile3();
    ~cFile3();

    typedef boost::function<void(sField::PtrT const &field)> OnFieldT;

    int Read(char const *fname,
             char const *pass,
             OnFieldT const &on_field);
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
