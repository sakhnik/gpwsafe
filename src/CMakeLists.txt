
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
	${APP_TARGET}
	main.cc
	App.cc App.hh
	Command.cc Command.hh
	CommandAdd.cc CommandAdd.hh
	CommandCreate.cc CommandCreate.hh
	CommandEdit.cc CommandEdit.hh
	CommandList.cc CommandList.hh
	Exceptions.hh
	IEmitter.hh
	StdoutEmitter.cc StdoutEmitter.hh
	Terminal.cc Terminal.hh
	)

target_link_libraries(
	${APP_TARGET}
	libgpwsafe
	${LIBGCRYPT_LIBRARIES}
	readline
	${Boost_PROGRAM_OPTIONS_LIBRARIES}
	)
