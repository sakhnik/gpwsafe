//
// Privileges.hh
//
//     Created: 02.02.2012
//      Author: A. Sakhnik
//

#pragma once

#include <unistd.h>
#include <boost/noncopyable.hpp>

namespace gPWS {

class cPrivileges
    : boost::noncopyable
{
public:
    static void Init();

    cPrivileges();
    ~cPrivileges();

private:
    static uid_t _saved_uid;
    static gid_t _saved_gid;
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
