//
// EntryTest.cc
//
//     Created: 02.02.2012
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


#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "../src/Entry.hh"

using namespace std;

BOOST_AUTO_TEST_SUITE(EntryTest)

BOOST_AUTO_TEST_CASE(TestDiff)
{
	using namespace gPWS;
	cEntry::PtrT entry0(cEntry::Create());
	entry0->SetGroup("arch");
	entry0->SetTitle("aur");

	cEntry::PtrT entry1(cEntry::Create());
	entry1->SetTitle("aur2");
	entry1->SetUser("sakhnik");

	cEntry::DiffT diff = entry1->Diff(entry0);

	BOOST_CHECK_EQUAL(diff.size(), 3);
	BOOST_CHECK_EQUAL(diff[0].type, cEntry::FT_GROUP);
	BOOST_CHECK_EQUAL(diff[0].change, cEntry::sChange::C_DELETED);
	BOOST_CHECK_EQUAL(diff[1].type, cEntry::FT_TITLE);
	BOOST_CHECK_EQUAL(diff[1].change, cEntry::sChange::C_MODIFIED);
	BOOST_CHECK_EQUAL(diff[2].type, cEntry::FT_USER);
	BOOST_CHECK_EQUAL(diff[2].change, cEntry::sChange::C_ADDED);
}

BOOST_AUTO_TEST_SUITE_END()

// vim: set noet ts=4 sw=4 tw=80:
