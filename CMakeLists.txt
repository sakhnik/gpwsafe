cmake_minimum_required(VERSION 2.8)

set(PACKAGE gpwsafe)
set(PACKAGE_VERSION "0.10.0")

project(${PACKAGE})

include(CTest)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

# FIXME:
#find_package(CXXFeatures)
add_definitions(-std=c++11 -DPACKAGE=\"${PACKAGE}\")
add_definitions(-DVERSION="${PACKAGE_VERSION}")

find_package(PkgConfig REQUIRED)

find_package(LibGcrypt 1.2 REQUIRED)

add_definitions(-DBOOST_DISABLE_THREADS)
find_package(Boost 1.48 REQUIRED COMPONENTS program_options unit_test_framework)

pkg_check_modules(GTK3 gtk+-3.0)
if(GTK3_FOUND)
	set(ENABLE_XCLIP True)
	add_definitions(-DENABLE_GTK -DENABLE_XCLIP)
	set(GTK_FOUND ${GTK3_FOUND})
	set(GTK_INCLUDE_DIRS ${GTK3_INCLUDE_DIRS})
	set(GTK_LIBRARY_DIRS ${GTK3_LIBRARY_DIRS})
	set(GTK_DEFINITIONS ${GTK3_DEFINITIONS})
	set(GTK_LIBRARIES ${GTK3_LIBRARIES})
else(GTK3_FOUND)
	set(ENABLE_XCLIP True)
	find_package(GTK2 COMPONENTS gtk)
	if(GTK2_FOUND)
		add_definitions(-DENABLE_GTK -DENABLE_XCLIP)
		set(GTK_FOUND ${GTK2_FOUND})
		set(GTK_INCLUDE_DIRS ${GTK2_INCLUDE_DIRS})
		set(GTK_LIBRARY_DIRS ${GTK2_LIBRARY_DIRS})
		set(GTK_DEFINITIONS ${GTK2_DEFINITIONS})
		set(GTK_LIBRARIES ${GTK2_LIBRARIES})
	endif()
endif(GTK3_FOUND)

find_package(Gettext)
message(STATUS "Found gettext version ${GETTEXT_VERSION_STRING}")

find_program(
	ASCIIDOC
	NAME a2x
	PATHS ENV PATH
	DOC "Asciidoc text processor"
	)
if(${ASCIIDOC} STREQUAL "ASCIIDOC-NOTFOUND")
	message(STATUS "Asciidoc not found. Skipping documentation.")
else()
	message(STATUS "Found asciidoc: ${ASCIIDOC}")
	add_subdirectory(doc)
endif()

add_subdirectory(src)
add_subdirectory(po)
add_subdirectory(test)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)
