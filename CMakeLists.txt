# -*- cmake -*-
# Copyright (C) Dmitry Igrishin
# For conditions of distribution and use, see files LICENSE.txt or internal.hpp

project(dmitigr_internal)

cmake_minimum_required(VERSION 3.13)
cmake_policy(VERSION 3.13)

if (NOT (UNIX OR WIN32))
  message(FATAL_ERROR "unsupported target platform")
endif()

list(APPEND CMAKE_MODULE_PATH ${dmitigr_internal_SOURCE_DIR}/cmake)
include(dmitigr)

set(dmitigr_internal_version_part1 0)
set(dmitigr_internal_version_part2 1)
add_definitions(-DDMITIGR_INTERNAL_VERSION_PART1=${dmitigr_internal_version_part1})
add_definitions(-DDMITIGR_INTERNAL_VERSION_PART2=${dmitigr_internal_version_part2})

# ------------------------------------------------------------------------------
# Build options
# ------------------------------------------------------------------------------

option(BUILD_SHARED_LIBS "Build shared library?" OFF)
option(DMITIGR_INTERNAL_HEADER_ONLY "Header-only library?" ON)
option(DMITIGR_INTERNAL_LIBRARIAN_DEBUG "Print librarian.cmake debug output?" OFF)

if(NOT DMITIGR_INTERNAL_HEADER_ONLY)
  if(BUILD_SHARED_LIBS)
    message("Building of shared library is enabled.")
    add_definitions(-DDMITIGR_INTERNAL_DLL_BUILDING)
  else()
    message("Building of static library is enabled.")
  endif()

  if(NOT DEFINED CMAKE_BUILD_TYPE OR NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING
      "Build type: Debug Release RelWithDebInfo MinSizeRel." FORCE)
  endif()
  message("Build type is ${CMAKE_BUILD_TYPE}")
else()
  message("Header-only mode is enabled")
endif()

if(DMITIGR_INTERNAL_LIBRARIAN_DEBUG)
  message("")
  message("Starting the debug output of librarian.cmake:")
  foreach(lib "GraphicsMagick" "Pq")
    find_package(${lib})
    include(librarian_debug)
  endforeach()
  message("Ending the debug output of librarian.cmake.")
  message("")
endif()

# ------------------------------------------------------------------------------
# Installation options
# ------------------------------------------------------------------------------

if(UNIX)
  set(DMITIGR_INTERNAL_CMAKE_INSTALL_DIR "share/dmitigr_internal/cmake" CACHE
    STRING "Name of the installation directory for the CMake stuff relative to ${CMAKE_INSTALL_PREFIX}")
  set(DMITIGR_INTERNAL_DOC_INSTALL_DIR "share/dmitigr_internal/doc" CACHE
    STRING "Name of the installation directory for the documentation relative to ${CMAKE_INSTALL_PREFIX}")
  set(DMITIGR_INTERNAL_BIN_INSTALL_DIR "bin" CACHE
    STRING "Name of the installation directory for the programs relative to ${CMAKE_INSTALL_PREFIX}")
  set(DMITIGR_INTERNAL_LIB_INSTALL_DIR "lib" CACHE
    STRING "Name of the installation directory for the libraries relative to ${CMAKE_INSTALL_PREFIX}")
  set(DMITIGR_INTERNAL_INCLUDE_INSTALL_DIR "include" CACHE
    STRING "Name of the installation directory for the includes relative to ${CMAKE_INSTALL_PREFIX}")
elseif(WIN32)
  # On Windows, CMAKE_INSTALL_PREFIX is $ENV{ProgramFiles}\\${CMAKE_PROJECT_NAME} by default. In turn:
  #   - on AMD64: ProgramFiles=%ProgramFiles%
  #   - on   x86: ProgramFiles=%ProgramFiles(x86)%
  # See: https://msdn.microsoft.com/en-us/library/aa384274.aspx
  set(DMITIGR_INTERNAL_CMAKE_INSTALL_DIR "cmake" CACHE
    STRING "Name of share directory relative to ${CMAKE_INSTALL_PREFIX}")
  set(DMITIGR_INTERNAL_DOC_INSTALL_DIR "doc" CACHE
    STRING "Name of the documentation directory relative to ${CMAKE_INSTALL_PREFIX}")
  set(DMITIGR_INTERNAL_BIN_INSTALL_DIR "bin" CACHE
    STRING "Name of the installation directory for the programs relative to ${CMAKE_INSTALL_PREFIX}")
  set(DMITIGR_INTERNAL_LIB_INSTALL_DIR "lib" CACHE
    STRING "Name of the library directory relative to ${CMAKE_INSTALL_PREFIX}")
  set(DMITIGR_INTERNAL_INCLUDE_INSTALL_DIR "include" CACHE
    STRING "Name of includes directory relative to ${CMAKE_INSTALL_PREFIX}")
endif()

# ------------------------------------------------------------------------------
# Languages
# ------------------------------------------------------------------------------

enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)

include_directories(BEFORE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR}/lib)

# ------------------------------------------------------------------------------
# Sources
# ------------------------------------------------------------------------------

set(dmitigr_internal_root_headers
  lib/dmitigr/internal.hpp
  )

set(dmitigr_internal_header_only_headers
  lib/dmitigr/internal/header_only.hpp
  )

set(dmitigr_internal_headers
  lib/dmitigr/internal/algorithm.hpp
  lib/dmitigr/internal/basics.hpp
  lib/dmitigr/internal/config.hpp
  lib/dmitigr/internal/console.hpp
  lib/dmitigr/internal/debug.hpp
  lib/dmitigr/internal/dll.hpp
  lib/dmitigr/internal/filesystem_experimental.hpp
  lib/dmitigr/internal/filesystem.hpp
  lib/dmitigr/internal/macros.hpp
  lib/dmitigr/internal/math.hpp
  lib/dmitigr/internal/memory.hpp
  lib/dmitigr/internal/net.hpp
  lib/dmitigr/internal/os.hpp
  lib/dmitigr/internal/stream.hpp
  lib/dmitigr/internal/string.hpp
  )

set(dmitigr_internal_transunits
  lib/dmitigr/internal/config.cpp
  lib/dmitigr/internal/console.cpp
  lib/dmitigr/internal/filesystem.cpp
  lib/dmitigr/internal/net.cpp
  lib/dmitigr/internal/os.cpp
  lib/dmitigr/internal/stream.cpp
  lib/dmitigr/internal/string.cpp
  )

set(dmitigr_internal_sources
  ${dmitigr_internal_root_headers}
  ${dmitigr_internal_header_only_headers}
  ${dmitigr_internal_headers}
  ${dmitigr_internal_transunits}
  )

# ------------------------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------------------------

# GraphicsMagick
find_package(GraphicsMagick)
if (GraphicsMagick_FOUND)
  message("Found GraphicsMagick++ headers at: ${GraphicsMagick_INCLUDE_DIRS}")
  list(APPEND dmitigr_internal_headers graphicsmagick.hpp)
  list(APPEND dmitigr_internal_transunits graphicsmagick.cpp)
  include_directories(${GraphicsMagick_INCLUDE_DIRS})
  if(NOT DMITIGR_INTERNAL_HEADER_ONLY)
    target_link_libraries(${dmitigr_internal_target} ${Suggested_GraphicsMagick_LIBRARIES})
  else()
    target_link_libraries(dmitigr_internal_interface INTERFACE ${Suggested_GraphicsMagick_LIBRARIES})
  endif()
  target_compile_definitions(${dmitigr_internal_target} PUBLIC -DDMITIGR_INTERNAL_GRAPHICSMAGICK)
endif()

# ------------------------------------------------------------------------------
# Targets
# ------------------------------------------------------------------------------

# Note: Multi-configuration generators (VS, Xcode) appends a per-configuration
# subdirectory to CMAKE_RUNTIME_OUTPUT_DIRECTORY unless a generator expression
# is used.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

if(NOT DMITIGR_INTERNAL_HEADER_ONLY)
  if(BUILD_SHARED_LIBS)
    set(dmitigr_internal_target "dmitigr_internal")
  else()
    set(dmitigr_internal_target "dmitigr_internal_static")
  endif()

  add_library(${dmitigr_internal_target} ${dmitigr_internal_sources})

  if(BUILD_SHARED_LIBS)
    target_compile_definitions(dmitigr_internal PUBLIC -DDMITIGR_INTERNAL_DLL)
  endif()

  set_target_properties(${dmitigr_internal_target}
    PROPERTIES
    LINKER_LANGUAGE "CXX"
    POSITION_INDEPENDENT_CODE True
    VERSION ${dmitigr_internal_version_part1}.${dmitigr_internal_version_part2}
    DEBUG_POSTFIX "d")

  dmitigr_target_compile_options(${dmitigr_internal_target})

  if (UNIX)
    target_link_libraries(${dmitigr_internal_target} stdc++fs)
  elseif (WIN32)
    target_link_libraries(${dmitigr_internal_target} Ws2_32.lib)
  endif()
else() # for header-only
  set(dmitigr_internal_target "dmitigr_internal_interface")

  add_library(dmitigr_internal_interface INTERFACE)

  target_compile_definitions(dmitigr_internal_interface INTERFACE -DDMITIGR_INTERNAL_HEADER_ONLY)

  if (UNIX)
    target_link_libraries(dmitigr_internal_interface INTERFACE stdc++fs)
  elseif (WIN32)
    target_link_libraries(dmitigr_internal_interface INTERFACE Ws2_32.lib)
  endif()
endif()

# ------------------------------------------------------------------------------
# Installing
# ------------------------------------------------------------------------------

install(PROGRAMS bin/dmitigr_internal_adapt
  DESTINATION ${DMITIGR_INTERNAL_BIN_INSTALL_DIR})

install(FILES bin/dmitigr_internal_basics.sh
  DESTINATION ${DMITIGR_INTERNAL_BIN_INSTALL_DIR})

# --------------------------------------

install(FILES ${dmitigr_internal_root_headers}
  DESTINATION ${DMITIGR_INTERNAL_INCLUDE_INSTALL_DIR}/dmitigr)

if(DMITIGR_INTERNAL_HEADER_ONLY)
  install(FILES ${dmitigr_internal_header_only_headers}
    DESTINATION ${DMITIGR_INTERNAL_INCLUDE_INSTALL_DIR}/dmitigr/internal)
endif()

install(FILES ${dmitigr_internal_headers}
  DESTINATION ${DMITIGR_INTERNAL_INCLUDE_INSTALL_DIR}/dmitigr/internal)

if(NOT DMITIGR_INTERNAL_HEADER_ONLY)
  install(TARGETS ${dmitigr_internal_target}
    EXPORT ${dmitigr_internal_target}_export
    ARCHIVE  DESTINATION ${DMITIGR_INTERNAL_LIB_INSTALL_DIR}
    LIBRARY  DESTINATION ${DMITIGR_INTERNAL_LIB_INSTALL_DIR}
    RUNTIME  DESTINATION ${DMITIGR_INTERNAL_LIB_INSTALL_DIR}
    INCLUDES DESTINATION ${DMITIGR_INTERNAL_INCLUDE_INSTALL_DIR})
else()
  install(TARGETS dmitigr_internal_interface
    EXPORT dmitigr_internal_interface_export
    INCLUDES DESTINATION ${DMITIGR_INTERNAL_INCLUDE_INSTALL_DIR})

  install(FILES ${dmitigr_internal_transunits}
    DESTINATION ${DMITIGR_INTERNAL_INCLUDE_INSTALL_DIR}/dmitigr/internal)
endif()

install(EXPORT ${dmitigr_internal_target}_export
  DESTINATION ${DMITIGR_INTERNAL_CMAKE_INSTALL_DIR}
  FILE ${dmitigr_internal_target}-config.cmake)

# ------------------------------------------------------------------------------
# Uninstalling
# ------------------------------------------------------------------------------

add_custom_target(uninstall)

add_custom_command(TARGET uninstall PRE_BUILD
  COMMAND cmake -E remove -f ${CMAKE_INSTALL_PREFIX}/${DMITIGR_INTERNAL_CMAKE_INSTALL_DIR}/dmitigr_internal*
  COMMAND cmake -E remove -f ${CMAKE_INSTALL_PREFIX}/${DMITIGR_INTERNAL_DOC_INSTALL_DIR}/dmitigr_internal*
  COMMAND cmake -E remove -f ${CMAKE_INSTALL_PREFIX}/${DMITIGR_INTERNAL_BIN_INSTALL_DIR}/dmitigr_internal*
  COMMAND cmake -E remove -f ${CMAKE_INSTALL_PREFIX}/${DMITIGR_INTERNAL_LIB_INSTALL_DIR}/dmitigr_internal*
  COMMAND cmake -E remove -f ${CMAKE_INSTALL_PREFIX}/${DMITIGR_INTERNAL_LIB_INSTALL_DIR}/libdmitigr_internal*
  COMMAND cmake -E remove -f ${CMAKE_INSTALL_PREFIX}/${DMITIGR_INTERNAL_INCLUDE_INSTALL_DIR}/dmitigr/internal*
  COMMAND cmake -E remove_directory ${CMAKE_INSTALL_PREFIX}/${DMITIGR_INTERNAL_INCLUDE_INSTALL_DIR}/dmitigr/internal
  )
