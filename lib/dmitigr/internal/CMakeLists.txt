# -*- cmake -*-
# Copyright (C) Dmitry Igrishin
# For conditions of distribution and use, see files LICENSE.txt or internal.hpp

# ------------------------------------------------------------------------------
# Sources
# ------------------------------------------------------------------------------

set(dmitigr_internal_sources
  algorithm.hpp
  basics.hpp
  config.cpp
  config.hpp
  console.cpp
  console.hpp
  debug.hpp
  filesystem.cpp
  filesystem_experimental.hpp
  filesystem.hpp
  macros.hpp
  math.hpp
  memory.hpp
  net.cpp
  net.hpp
  os.cpp
  os.hpp
  stream.cpp
  stream.hpp
  string.cpp
  string.hpp
)

set_source_files_properties(
  dll.hpp.in

  PROPERTIES
  HEADER_FILE_ONLY ON)

# ------------------------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------------------------

set(CMAKE_MODULE_PATH ${dmitigr_internal_SOURCE_DIR}/cmake)

#
# GraphicsMagick
#

find_package(GraphicsMagick)
if (GraphicsMagick_FOUND)
  message("Found GraphicsMagick++ headers at: ${GraphicsMagick_INCLUDE_DIRS}")
  list(APPEND dmitigr_internal_sources graphicsmagick.cpp graphicsmagick.hpp)
  include_directories(${GraphicsMagick_INCLUDE_DIRS})
  if(BUILD_SHARED_LIBS)
    target_link_libraries(dmitigr_internal ${Suggested_GraphicsMagick_LIBRARIES})
  endif()
endif()

# ------------------------------------------------------------------------------
# Target tuning
# ------------------------------------------------------------------------------

# Note: Multi-configuration generators (VS, Xcode) appends a per-configuration
# subdirectory to CMAKE_RUNTIME_OUTPUT_DIRECTORY unless a generator expression
# is used.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

add_library(dmitigr_internal ${dmitigr_internal_sources})
set_target_properties(dmitigr_internal
  PROPERTIES
  LINKER_LANGUAGE "CXX"
  VERSION ${INTERNAL_VERSION_PART1}.${INTERNAL_VERSION_PART2}
  DEBUG_POSTFIX "d")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # FIXME: GDB 7.7 doesn't print info of rvalues' that are function arguments
    # properly when compiled with -gdwarf-4. So, using -gdwarf-3 instead.
    target_compile_options(dmitigr_internal PRIVATE
      -gdwarf-3
      -pedantic
      -Wall
      -Wextra
      -Winline
      -Winit-self
      -Wuninitialized
      -Wmaybe-uninitialized
      -Woverloaded-virtual
      -Wsuggest-override
      -Wlogical-op
      -Wswitch)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(dmitigr_internal PRIVATE
      -pedantic
      -Weverything)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(dmitigr_internal PRIVATE
      /Zc:throwingNew,referenceBinding,noexceptTypes
      /errorReport:none
      /nologo
      /utf-8)
  endif()
endif()

if(BUILD_SHARED_LIBS)
  if (UNIX)
    target_link_libraries(dmitigr_internal stdc++fs)
  elseif (WIN32)
    target_link_libraries(dmitigr_internal Ws2_32.lib)
  endif()
endif()

# ------------------------------------------------------------------------------
# Preprocessing
# ------------------------------------------------------------------------------

if(BUILD_SHARED_LIBS)
  configure_file(dll.hpp.in ${CMAKE_CURRENT_SOURCE_DIR}/dll.hpp @ONLY
    NEWLINE_STYLE UNIX)
endif()

# ------------------------------------------------------------------------------
# Installations
# ------------------------------------------------------------------------------

install(FILES
  algorithm.hpp
  basics.hpp
  config.hpp
  console.hpp
  debug.hpp
  filesystem_experimental.hpp
  filesystem.hpp
  graphicsmagick.hpp
  macros.hpp
  math.hpp
  memory.hpp
  net.hpp
  os.hpp
  stream.hpp
  string.hpp

  DESTINATION ${INTERNAL_INCLUDES_INSTALL_DIR}/dmitigr/internal)

install(TARGETS dmitigr_internal
  EXPORT dmitigr_internal_export
  ARCHIVE  DESTINATION ${INTERNAL_LIBRARY_INSTALL_DIR}
  LIBRARY  DESTINATION ${INTERNAL_LIBRARY_INSTALL_DIR}
  RUNTIME  DESTINATION ${INTERNAL_LIBRARY_INSTALL_DIR}
  INCLUDES DESTINATION ${INTERNAL_INCLUDES_INSTALL_DIR})

install(EXPORT dmitigr_internal_export
  DESTINATION ${INTERNAL_CMAKE_INSTALL_DIR}
  FILE dmitigr_internal-config.cmake)
