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


set(DOMAIN ${PACKAGE})
set(LINGUAS uk)
set(_pot_file ${CMAKE_CURRENT_BINARY_DIR}/messages.pot)

set(_gmo_files)

foreach(LANG ${LINGUAS})
	set(_po ${CMAKE_CURRENT_SOURCE_DIR}/${LANG}.po)
	if(EXISTS ${_po})
		set(_gmo ${CMAKE_CURRENT_BINARY_DIR}/${LANG}.gmo)
		add_custom_command(
			OUTPUT ${_gmo}
			MAIN_DEPENDENCY ${_po}
			COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -c -o ${_gmo} ${_po}
			)
		install(
			FILES ${_gmo}
			DESTINATION share/locale/${LANG}/LC_MESSAGES
			RENAME ${DOMAIN}.mo
			)
		list(APPEND _gmo_files ${_gmo})
	endif(EXISTS ${_po})
endforeach()

add_custom_target(translation ALL DEPENDS ${_gmo_files})


file(GLOB _src ../src/*.cc)

add_custom_command(
	OUTPUT ${_pot_file}
	COMMAND xgettext --c++ -k_ -o ${_pot_file} ${_src}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	)

set(_po_files)

foreach(LANG ${LINGUAS})
	set(_po ${CMAKE_CURRENT_SOURCE_DIR}/${LANG}.po)
	if(EXISTS ${_po})
		set(_po_file_new ${CMAKE_CURRENT_BINARY_DIR}/${LANG}.po)
		add_custom_command(
			OUTPUT ${_po_file_new}
			MAIN_DEPENDENCY ${_pot_file}
			COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE} ${_po} ${_pot_file} -o ${_po_file_new}
			COMMAND cp -f ${_po_file_new} ${_po}
			)
		list(APPEND _po_files ${_po_file_new})
	endif(EXISTS ${_po})
endforeach()

add_custom_target(update-po DEPENDS ${_po_files})
