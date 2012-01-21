//
// Entry.hh
//
//     Created: 21.01.2012
//      Author: A. Sakhnik
//

#pragma once

#include "Field.hh"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>

namespace gPWS {

class cEntry
    : boost::noncopyable
{
public:
    typedef boost::shared_ptr<cEntry> PtrT;

    void *operator new(size_t n);
    void operator delete(void *p, size_t n);

    bool AddField(sField::PtrT const &field);

    void Dump() const;

private:
    StringX _group;
    StringX _title;
    StringX _user;
    StringX _notes;
    StringX _pass;
    StringX _url;

    typedef std::vector<sField::PtrT> _OtherT;
    _OtherT _other;
};

} //namespace gPWS;

// vim: set et ts=4 sw=4:
