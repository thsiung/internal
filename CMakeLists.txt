# -*- cmake -*-
# Copyright (C) Dmitry Igrishin
# For conditions of distribution and use, see files LICENSE.txt or internal.hpp

project(dmitigr_internal)

cmake_minimum_required(VERSION 3.13)
cmake_policy(VERSION 3.13)

if (NOT (UNIX OR WIN32))
  message(FATAL_ERROR "unsupported target platform")
endif()

set(INTERNAL_VERSION_PART1 0)
set(INTERNAL_VERSION_PART2 1)
add_definitions(-DDMITIGR_INTERNAL_VERSION_PART1=${VERSION_PART1})
add_definitions(-DDMITIGR_INTERNAL_VERSION_PART2=${VERSION_PART2})

# ------------------------------------------------------------------------------
option(DMITIGR_INTERNAL_HEADER_ONLY "Header-only library?" ON)

if(NOT DMITIGR_INTERNAL_HEADER_ONLY)
  # Only static library can be build now.
  set(BUILD_SHARED_LIBS OFF)

  if(BUILD_SHARED_LIBS)
    message("Building shared library is enabled.")
    add_definitions(-DDMITIGR_INTERNAL_DLL_BUILDING)
  else()
    message("Building shared library is disabled.")
  endif()

  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug
      CACHE
      STRING "Build type: Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
  endif()
  message("Build type is ${CMAKE_BUILD_TYPE}")
else()
  message("Header-only mode is enabled")
  add_definitions(-DDMITIGR_INTERNAL_HEADER_ONLY)
endif()

# ------------------------------------------------------------------------------

if(UNIX)
  set(INTERNAL_CMAKE_INSTALL_DIR "share/dmitigr_internal/cmake" CACHE
    STRING "Name of the cmake directory relative to ${CMAKE_INSTALL_PREFIX}")
  set(INTERNAL_DOC_INSTALL_DIR "share/dmitigr_internal/doc" CACHE
    STRING "Name of the documentation directory relative to ${CMAKE_INSTALL_PREFIX}")
  set(INTERNAL_LIBRARY_INSTALL_DIR "lib" CACHE
    STRING "Name of the library directory relative to ${CMAKE_INSTALL_PREFIX}")
  set(INTERNAL_INCLUDES_INSTALL_DIR "include" CACHE
    STRING "Name of includes directory relative to ${CMAKE_INSTALL_PREFIX}")
elseif(WIN32)
  # On Windows, CMAKE_INSTALL_PREFIX is $ENV{ProgramFiles}\\${CMAKE_PROJECT_NAME} by default. In turn:
  #   - on AMD64: ProgramFiles=%ProgramFiles%
  #   - on   x86: ProgramFiles=%ProgramFiles(x86)%
  # See: https://msdn.microsoft.com/en-us/library/aa384274.aspx
  set(INTERNAL_CMAKE_INSTALL_DIR "cmake" CACHE
    STRING "Name of share directory relative to ${CMAKE_INSTALL_PREFIX}")
  set(INTERNAL_DOC_INSTALL_DIR "doc" CACHE
    STRING "Name of the documentation directory relative to ${CMAKE_INSTALL_PREFIX}")
  set(INTERNAL_LIBRARY_INSTALL_DIR "lib" CACHE
    STRING "Name of the library directory relative to ${CMAKE_INSTALL_PREFIX}")
  set(INTERNAL_INCLUDES_INSTALL_DIR "include" CACHE
    STRING "Name of includes directory relative to ${CMAKE_INSTALL_PREFIX}")
endif()

# ------------------------------------------------------------------------------

add_custom_target(uninstall)

add_custom_command(TARGET uninstall PRE_BUILD
  COMMAND cmake -E remove -f ${CMAKE_INSTALL_PREFIX}/${INTERNAL_LIBRARY_INSTALL_DIR}/libdmitigr_internal*
  COMMAND cmake -E remove -f ${CMAKE_INSTALL_PREFIX}/${INTERNAL_LIBRARY_INSTALL_DIR}/dmitigr_internal*.dll
  COMMAND cmake -E remove -f ${CMAKE_INSTALL_PREFIX}/${INTERNAL_LIBRARY_INSTALL_DIR}/dmitigr_internal*.lib

  COMMAND cmake -E remove -f ${CMAKE_INSTALL_PREFIX}/${INTERNAL_INCLUDES_INSTALL_DIR}/dmitigr/internal.hpp
  COMMAND cmake -E remove_directory ${CMAKE_INSTALL_PREFIX}/${INTERNAL_INCLUDES_INSTALL_DIR}/dmitigr/internal
  COMMAND cmake -E remove_directory ${CMAKE_INSTALL_PREFIX}/${INTERNAL_CMAKE_INSTALL_DIR}
  COMMAND cmake -E remove_directory ${CMAKE_INSTALL_PREFIX}/${INTERNAL_DOC_INSTALL_DIR}
  )

# ------------------------------------------------------------------------------

add_subdirectory(bin)
add_subdirectory(cmake)
add_subdirectory(lib)
