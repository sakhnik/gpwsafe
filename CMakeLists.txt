cmake_minimum_required(VERSION 2.8)

set(APP_TARGET gpwsafe)
set(GPWSAFE_VERSION "0.10.0")

project(${APP_TARGET})

include(CTest)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules")

# FIXME:
#find_package(CXXFeatures)
add_definitions(-std=c++11 -DPACKAGE=\"${APP_TARGET}\")
add_definitions(-DVERSION="${GPWSAFE_VERSION}")

find_package(LibGcrypt 1.2 REQUIRED)

add_definitions(-DBOOST_DISABLE_THREADS)
find_package(Boost 1.48 REQUIRED COMPONENTS program_options unit_test_framework)

find_package(Gettext)
message(STATUS "Found gettext version ${GETTEXT_VERSION_STRING}")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)
add_subdirectory(po)
add_subdirectory(test)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)
