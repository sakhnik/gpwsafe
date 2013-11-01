# This file is part of gpwsafe.
#
# Gpwsafe is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Gpwsafe is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with gpwsafe.  If not, see <http://www.gnu.org/licenses/>

check_PROGRAMS = \
	KeyStretchTest \
	File3Test \
	EntryTest

KeyStretchTest_SOURCES = KeyStretchTest.cc
KeyStretchTest_LDADD = \
	$(top_builddir)/src/libgpwsafe.a \
	$(LIBGCRYPT_LIBS) \
	$(BOOST_UNIT_TEST_FRAMEWORK_LIB)

File3Test_SOURCES = File3Test.cc
File3Test_LDADD = \
	$(top_builddir)/src/libgpwsafe.a \
	$(LIBGCRYPT_LIBS) \
	$(BOOST_UNIT_TEST_FRAMEWORK_LIB)

EntryTest_SOURCES = EntryTest.cc
EntryTest_LDADD = \
	$(top_builddir)/src/libgpwsafe.a \
	$(LIBGCRYPT_LIBS) \
	$(BOOST_UNIT_TEST_FRAMEWORK_LIB)

AM_CPPFLAGS = $(BOOST_CPPFLAGS) -DBOOST_TEST_DYN_LINK


TESTS = $(check_PROGRAMS)

if HAVE_PERL_EXPECT
TESTS += generic_test.pl
endif  # HAVE_PERL_EXPECT
