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


add_definitions(-Wall -Werror -DGCRYPT_NO_DEPRECATED)

# FIXME
add_definitions(-DLOCALEDIR=\"${CMAKE_INSTALL_PREFIX}/share/locale\")

add_library(
	libgpwsafe
	STATIC
	Database.cc Database.hh
	Debug.cc Debug.hh
	Defs.hh
	Entry.cc Entry.hh
	Field.cc Field.hh
	File3.cc File3.hh
	Gcrypt.cc Gcrypt.hh
	Hmac.cc Hmac.hh
	KeyStretch.cc KeyStretch.hh
	Memory.cc Memory.hh
	Random.cc Random.hh
	Sha256.cc Sha256.hh
	Twofish.cc Twofish.hh
	i18n.h
	)

add_executable(
	${PACKAGE}
	main.cc
	App.cc App.hh
	Command.cc Command.hh
	CommandAdd.cc CommandAdd.hh
	CommandCreate.cc CommandCreate.hh
	CommandDelete.cc CommandDelete.hh
	CommandEdit.cc CommandEdit.hh
	CommandList.cc CommandList.hh
	Exceptions.hh
	IEmitter.hh
	StdoutEmitter.cc StdoutEmitter.hh
	Terminal.cc Terminal.hh
	)

target_link_libraries(
	${PACKAGE}
	libgpwsafe
	${LIBGCRYPT_LIBRARIES}
	readline
	ncurses
	${Boost_PROGRAM_OPTIONS_LIBRARIES}
	${Boost_FILESYSTEM_LIBRARIES}
	${Boost_SYSTEM_LIBRARIES}
	)

if(GTK_FOUND)
	#link_directories(${GTK_LIBRARY_DIRS})

	add_library(gtk_emitter STATIC GtkEmitter.cc GtkEmitter.hh)
	set_property(
		TARGET gtk_emitter
		PROPERTY COMPILE_DEFINITIONS ${GTK_DEFINITIONS}
		PROPERTY INCLUDE_DIRECTORIES ${GTK_INCLUDE_DIRS}
		)

	target_link_libraries(
		${PACKAGE}
		gtk_emitter
		${GTK_LIBRARIES}
		)
endif()

install(TARGETS ${PACKAGE} DESTINATION bin)
