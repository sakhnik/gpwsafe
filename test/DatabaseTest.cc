//
// DatabaseTest.cc
//
//     Created: 15.03.2014
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

#define private public
#include "../src/Database.hh"
#undef private
#include "../src/Gcrypt.hh"

using namespace std;
using namespace gPWS;

struct sGlobalFixture
{
	sGlobalFixture()
	{
		gPWS::cGcrypt::Init(true);
	}

} global_fixture;

TEST(TestDatabase, FollowSymlink)
{
#define DEFS \
		"#/bin/bash\n" \
		"d=/tmp/gpws\n" \
		"t=$d/sym/link/1.psafe3\n" \
		"f=$d/f.psafe3\n"

	ASSERT_EQ(
		0,
		system(DEFS
		       "rm -rf $d\n"
		       "mkdir -p `dirname $t`\n"
		       "echo asdf > $t\n"
		       "ln -s $t $f\n")
		);

	cDatabase d;
	d._fname = "/tmp/gpws/f.psafe3";
	d._pass = "password";
	d.Write();

	EXPECT_EQ(0, system(DEFS "test asdf = `cat $t~`\n"));
	EXPECT_EQ(0, system(DEFS "test $t = `readlink -f $f`\n"));

	system("rm -rf /tmp/gpws");
#undef DEFS
}

TEST(TestDatabase, FollowSymlinkRelative)
{
#define DEFS \
		"#/bin/bash\n" \
		"d=/tmp/gpws\n" \
		"t=sym/link/1.psafe3\n" \
		"f=$d/f.psafe3\n"

	ASSERT_EQ(
		0,
		system(DEFS
		       "rm -rf $d\n"
		       "mkdir -p `dirname $d/$t`\n"
		       "echo asdf > $d/$t\n"
		       "ln -sf $t $f\n")
		);

	cDatabase d;
	d._fname = "/tmp/gpws/f.psafe3";
	d._pass = "password";
	d.Write();

	EXPECT_EQ(0, system(DEFS "test asdf = `cat $d/$t~`\n"));
	EXPECT_EQ(0, system(DEFS "test $d/$t = `readlink -f $f`\n"));

	system("rm -rf /tmp/gpws");
#undef DEFS
}

// vim: set noet ts=4 sw=4 tw=80:
