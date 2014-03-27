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

add_subdirectory(/usr/src/gtest gtest)

set(UNIT_TESTS
	KeyStretchTest
	File3Test
	DatabaseTest
	EntryTest
	)

set(LIBRARIES libgpwsafe ${LIBGCRYPT_LIBRARIES} gtest gtest_main)
add_definitions(-DBOOST_TEST_DYN_LINK)

foreach(TEST ${UNIT_TESTS})
	add_executable(${TEST} ${TEST}.cc)
	target_link_libraries(${TEST} ${LIBRARIES})
	add_test(NAME ${TEST} COMMAND ${TEST})
endforeach()

if(NOT ${PERLMODULES_EXPECT_MODULE} STREQUAL "NOTFOUND")
	add_test(
		NAME functional.generic 
		COMMAND perl ${CMAKE_CURRENT_SOURCE_DIR}/generic_test.pl
		)
endif()
