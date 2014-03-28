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


set(ASCIIDOC_OPTS -a version='${PACKAGE_VERSION}')
set(ASCIIDOC_OPTS ${ASCIIDOC_OPTS} -a date=`date +%Y-%m-%d`)

if(ENABLE_XCLIP)
set(ASCIIDOC_OPTS ${ASCIIDOC_OPTS} -a enable_xclip='yes')
endif(ENABLE_XCLIP)

set(A2X_OPTS -d manpage -f manpage)

set(_out ${CMAKE_CURRENT_BINARY_DIR}/gpwsafe.1)
set(_src ${CMAKE_CURRENT_SOURCE_DIR}/gpwsafe.1.txt)

if(${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
	set(_in ${_src})
else()
	set(_in ${CMAKE_CURRENT_BINARY_DIR}/gpwsafe.1.txt)
	add_custom_command(
		OUTPUT ${_in}
		MAIN_DEPENDENCY ${_src}
		COMMAND ln -s ${_src} ${_in}
		)
endif()

add_custom_command(
	OUTPUT ${_out}.gz
	MAIN_DEPENDENCY ${_in}
	COMMAND ${ASCIIDOC} ${A2X_OPTS}
	    --asciidoc-opts="${ASCIIDOC_OPTS} --out-file=${_out}.xml"
	    ${_in}
	COMMAND gzip -f ${_out}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	)

add_custom_target(manpage ALL DEPENDS ${_out}.gz)

install(FILES ${_out}.gz DESTINATION share/man/man1)
