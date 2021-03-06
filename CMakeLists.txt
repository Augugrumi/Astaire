cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)

project(astaire)

INCLUDE(CheckLibraryExists)
INCLUDE(FindPkgConfig)

# CXX11
set(CMAKE_CXX_STANDARD 11)
set(VERSION "0.1")

# Check if building in release or debug mode
if ((CMAKE_BUILD_TYPE MATCHES Debug) OR (CMAKE_BUILD_TYPE MATCHES DEBUG))
    set(DEBUG_BUILD 1)
    message(STATUS "Note: building in debug mode. Log will be more verbose")
else()
    set(DEBUG_BUILD 0)
    message(STATUS "Note: building in release mode. Build in debug mode for a verbose log")
endif()


# Where astaireopenvpn will be installed
set(CMAKE_INSTALL_BINDIR bin)

# Don't want compilation in source dir
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)
if(",${CMAKE_SOURCE_DIR}," STREQUAL ",${CMAKE_BINARY_DIR},")
    message(FATAL_ERROR "ERROR: In-source builds are not allowed.")
endif()

add_subdirectory("${astaire_SOURCE_DIR}/src/")
