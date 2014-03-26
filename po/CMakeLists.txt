
set(DOMAIN ${APP_TARGET})
set(LINGUAS uk)
set(LOCALE_INSTALL_DIR locale)
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
			DESTINATION ${LOCALE_INSTALL_DIR}/${LANG}/LC_MESSAGES
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
