//
// Entry.hh
//
//     Created: 21.01.2012
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

#pragma once

#include "Field.hh"

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>

namespace gPWS {

class cEntry
    : boost::noncopyable
{
public:
    typedef boost::shared_ptr<cEntry> PtrT;

    void *operator new(size_t n);
    void operator delete(void *p, size_t n);

    bool AddField(sField::PtrT const &field);

    typedef boost::function<void(sField::PtrT const &ptr)> OnFieldT;

    void ForEachField(OnFieldT on_field);

    void Dump() const;

    StringX const &GetGroup() const { return _group; }
    StringX const &GetTitle() const { return _title; }
    StringX const &GetUser() const { return _user; }
    StringX const &GetNotes() const { return _notes; }
    StringX const &GetPass() const { return _pass; }
    StringX const &GetUrl() const { return _url; }

    // group.title
    StringX GetFullTitle() const;

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
