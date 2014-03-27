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

enable_testing()

set(UNIT_TESTS
	KeyStretchTest
	File3Test
	DatabaseTest
	EntryTest
	)

set(LIBRARIES libgpwsafe ${LIBGCRYPT_LIBRARIES} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARIES})
add_definitions(-DBOOST_TEST_DYN_LINK)

foreach(TEST ${UNIT_TESTS})
	add_executable(${TEST} ${TEST}.cc)
	target_link_libraries(${TEST} ${LIBRARIES})
	add_test(NAME ${TEST} COMMAND ${TEST})
endforeach()

#if HAVE_PERL_EXPECT
#TESTS += generic_test.pl
#endif  # HAVE_PERL_EXPECT
