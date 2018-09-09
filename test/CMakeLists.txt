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

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(UNIT_TESTS
	KeyStretchTest
	File3Test
	DatabaseTest
	EntryTest
	)

set(LIBRARIES libgpwsafe
	${LIBGCRYPT_LIBRARIES}
	${Boost_SYSTEM_LIBRARIES} ${Boost_FILESYSTEM_LIBRARIES}
	${GTEST_BOTH_LIBRARIES}
  ${CMAKE_THREAD_LIBS_INIT}
  )

foreach(TEST ${UNIT_TESTS})
	add_executable(${TEST} ${TEST}.cc)
	target_link_libraries(${TEST} ${LIBRARIES})
	add_test(NAME ${TEST} COMMAND ${TEST})
endforeach()

add_test(
	NAME functional.generic
	COMMAND bash
		${CMAKE_CURRENT_SOURCE_DIR}/generic_test.sh
		${CMAKE_BINARY_DIR}/src/gpwsafe
	)
