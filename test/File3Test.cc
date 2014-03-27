//
// File3Test.cc
//
//     Created: 29.01.2012
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

#include "../src/File3.hh"
#include "../src/Defs.hh"
#include "../src/Gcrypt.hh"
#include "../src/Debug.hh"

#include <boost/iterator/indirect_iterator.hpp>
#include <boost/scope_exit.hpp>

using namespace std;

struct sGlobalFixture
{
	sGlobalFixture()
	{
		gPWS::cGcrypt::Init(true);
	}

} global_fixture;


TEST(TestFile3, WriteRead)
{
	srandom(static_cast<unsigned>(time(NULL)));

	using namespace gPWS;

	typedef vector<sField::PtrT> FieldsT;
	FieldsT fields1;
	for (unsigned i = 0, n = random() % 999; i != n; ++i)
	{
		sField::PtrT field(sField::Create());
		field->type = random() % 256;
		field->value.resize(random() % 256);
		generate(field->value.begin(), field->value.end(), &random);
		fields1.push_back(field);
	}

	char const *const fname = "/tmp/file3_test.psafe3";
	char const *const pass = "!23$QweR";

	cFile3 file1;
	file1.OpenWrite(fname, pass, true);
	BOOST_SCOPE_EXIT((&fname)) {
		::unlink(fname);
	} BOOST_SCOPE_EXIT_END

	for (FieldsT::const_iterator i = fields1.begin(); i != fields1.end(); ++i)
		file1.WriteField(*i);

	file1.CloseWrite();

	cFile3 file2;
	file2.OpenRead(fname, pass);
	FieldsT fields2;
	sField::PtrT field;
	while ((field = file2.ReadField()))
	{
		fields2.push_back(field);
	}

	ASSERT_EQ(fields1.size(), fields2.size());

	for (FieldsT::const_iterator it1{begin(fields1)}, it2{begin(fields2)};
	     it1 != end(fields1);
	     ++it1, ++it2)
	{
		const sField &f1 = *(*it1);
		const sField &f2 = *(*it2);

		EXPECT_EQ(f1, f2);
		EXPECT_EQ(f1.type, f2.type);
		EXPECT_EQ(f1.value, f2.value);
	}
}

// vim: set noet ts=4 sw=4 tw=80:
