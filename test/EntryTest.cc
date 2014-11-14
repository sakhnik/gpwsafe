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


#include <gtest/gtest.h>
#include "../src/Entry.hh"

using namespace std;

TEST(TestEntry, Diff)
{
	using namespace gPWS;
	Entry::PtrT entry0(Entry::Create());
	entry0->SetGroup("arch");
	entry0->SetTitle("aur");

	Entry::PtrT entry1(Entry::Create());
	entry1->SetTitle("aur2");
	entry1->SetUser("sakhnik");

	Entry::DiffT diff = entry1->Diff(entry0);

	ASSERT_EQ(3, diff.size());
	EXPECT_EQ(diff[0].type, Entry::FT_GROUP);
	EXPECT_EQ(diff[0].change, Entry::sChange::C_DELETED);
	EXPECT_EQ(diff[1].type, Entry::FT_TITLE);
	EXPECT_EQ(diff[1].change, Entry::sChange::C_MODIFIED);
	EXPECT_EQ(diff[2].type, Entry::FT_USER);
	EXPECT_EQ(diff[2].change, Entry::sChange::C_ADDED);
}

// vim: set noet ts=4 sw=4 tw=80:
