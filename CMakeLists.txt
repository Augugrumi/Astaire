cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)

project(astaire)

INCLUDE(CheckLibraryExists)
INCLUDE(FindPkgConfig)

# CXX11
set(CMAKE_CXX_STANDARD 11)
set(VERSION "0.1")

# Where astaireopenvpn will be installed
set(CMAKE_INSTALL_BINDIR bin)
add_subdirectory("${astaire_SOURCE_DIR}/src/")

# Don't want compilation in source dir
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)
if(",${CMAKE_SOURCE_DIR}," STREQUAL ",${CMAKE_BINARY_DIR},")
    message(FATAL_ERROR "ERROR: In-source builds are not allowed.")
endif()
