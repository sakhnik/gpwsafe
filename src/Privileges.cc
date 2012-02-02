//
// Privileges.cc
//
//     Created: 02.02.2012
//      Author: A. Sakhnik
//

#include "Privileges.hh"

namespace gPWS {

uid_t cPrivileges::_saved_uid(0);
gid_t cPrivileges::_saved_gid(0);

void cPrivileges::Init()
{
    _saved_uid = geteuid();
    _saved_gid = getegid();
    // if we are running suid, drop privileges now; we use seteuid()
    // instead of setuid() so the saved uid remains root and we can become
    // root again in order to mlock()
    if (_saved_uid != getuid() || _saved_gid != getgid())
    {
        setegid(getgid());
        seteuid(getuid());
    }
}

cPrivileges::cPrivileges()
{
    if (_saved_uid != geteuid())
        seteuid(_saved_uid);
    if (_saved_gid != getegid())
        setegid(_saved_gid);
}

cPrivileges::~cPrivileges()
{
    setegid(getgid());
    seteuid(getuid());
}

} //namespace gPWS;

// vim: set et ts=4 sw=4:
