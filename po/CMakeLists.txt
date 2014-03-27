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
set(POT_FILE ${CMAKE_CURRENT_BINARY_DIR}/messages.pot)

set(_gmoFiles)

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
		list(APPEND _gmoFiles ${_gmo})
	endif(EXISTS ${_po})
endforeach()

add_custom_target(translation ALL DEPENDS ${_gmoFiles})


file(GLOB _src ../src/*.cc)

#foreach(LANG ${LINGUAS})
#	set(_po ${LANG}.po)
#	if(EXISTS ${_po})
#		set(PO_FILE_NEW ${CMAKE_CURRENT_BINARY_DIR}/${LANG}.po)
#		set(GMO_FILE_NEW ${CMAKE_CURRENT_BINARY_DIR}/${LANG}.gmo)
#		add_custom_command(
#			TARGET translation
#			COMMAND ${GETTEXT_MSGMERGE_EXECUTABLE} ${_po} ${POT_FILE} -o ${PO_FILE_NEW}
#			COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -c -o ${GMO_FILE_NEW} ${PO_FILE_NEW}
#			)
#		install(
#			FILES ${GMO_FILE_NEW}
#			DESTINATION ${LOCALE_INSTALL_DIR}/${LANG}/LC_MESSAGES
#			RENAME ${DOMAIN}.mo
#			)
#	endif(EXISTS ${_po})
#endforeach()
