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

cmake_minimum_required(VERSION 2.8)

set(PACKAGE gpwsafe)
set(PACKAGE_VERSION "0.20.2")

project(${PACKAGE})

include(CTest)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

# FIXME:
#find_package(CXXFeatures)
add_definitions(-std=c++14 -DPACKAGE=\"${PACKAGE}\")
add_definitions(-DVERSION="${PACKAGE_VERSION}")

find_package(PkgConfig REQUIRED)

find_package(LibGcrypt 1.2 REQUIRED)

add_definitions(-DBOOST_DISABLE_THREADS)
find_package(Boost 1.48 REQUIRED COMPONENTS program_options filesystem system)

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

find_package(PythonInterp 3 REQUIRED)

# Find if a Python module is installed
# Found at http://www.cmake.org/pipermail/cmake/2011-January/041666.html
# To use do: find_python_module(PyQt4 REQUIRED)
function(find_python_module module)
	string(TOUPPER ${module} module_upper)
	if(NOT PY_${module_upper})
		if(ARGC GREATER 1 AND ARGV1 STREQUAL "REQUIRED")
			set(${module}_FIND_REQUIRED TRUE)
		endif()
		# A module's location is usually a directory, but for binary modules
		# it's a .so file.
		execute_process(COMMAND "${PYTHON_EXECUTABLE}" "-c" 
			"import re, ${module}; print(re.compile('/__init__.py.*').sub('',${module}.__file__))"
			RESULT_VARIABLE _${module}_status 
			OUTPUT_VARIABLE _${module}_location
			ERROR_QUIET 
			OUTPUT_STRIP_TRAILING_WHITESPACE)
		if(NOT _${module}_status)
			set(PY_${module_upper} ${_${module}_location} CACHE STRING 
				"Location of Python module ${module}")
		endif(NOT _${module}_status)
	endif(NOT PY_${module_upper})
	find_package_handle_standard_args(PY_${module} DEFAULT_MSG PY_${module_upper})
endfunction(find_python_module)

find_python_module(pexpect REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
configure_file(.ycm_extra_conf.py.in .ycm_extra_conf.py)

add_subdirectory(src)
add_subdirectory(po)
add_subdirectory(test)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)
