set(ASCIIDOC_OPTS -a version='${PACKAGE_VERSION}')
set(ASCIIDOC_OPTS ${ASCIIDOC_OPTS} -a date=`date +%Y-%m-%d`)

if(ENABLE_XCLIP)
set(ASCIIDOC_OPTS ${ASCIIDOC_OPTS} -a enable_xclip='yes')
endif(ENABLE_XCLIP)

set(A2X_OPTS -d manpage -f manpage)

set(_out ${CMAKE_CURRENT_BINARY_DIR}/gpwsafe.1)
set(_src ${CMAKE_CURRENT_SOURCE_DIR}/gpwsafe.1.txt)
set(_in ${CMAKE_CURRENT_BINARY_DIR}/gpwsafe.1.txt)

add_custom_command(
	OUTPUT ${_in}
	MAIN_DEPENDENCY ${_src}
	COMMAND ln -s ${_src} ${_in}
	)

add_custom_command(
	OUTPUT ${_out}.gz
	MAIN_DEPENDENCY ${_in}
	COMMAND ${ASCIIDOC} ${A2X_OPTS}
	    --asciidoc-opts="${ASCIIDOC_OPTS} --out-file=${_out}.xml"
	    ${_in}
	COMMAND gzip ${_out}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	)
add_custom_target(manpage ALL DEPENDS ${_out}.gz)
