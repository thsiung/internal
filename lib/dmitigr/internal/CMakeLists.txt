# -*- cmake -*-
# Copyright (C) Dmitry Igrishin
# For conditions of distribution and use, see files LICENSE.txt or internal.hpp

# ------------------------------------------------------------------------------
# Sources
# ------------------------------------------------------------------------------

set(dmitigr_internal_headers
  algorithm.hpp
  basics.hpp
  config.hpp
  console.hpp
  debug.hpp
  filesystem_experimental.hpp
  filesystem.hpp
  header_only.hpp
  macros.hpp
  math.hpp
  memory.hpp
  net.hpp
  os.hpp
  stream.hpp
  string.hpp
)

set(dmitigr_internal_implementations
  config.cpp
  console.cpp
  filesystem.cpp
  net.cpp
  os.cpp
  stream.cpp
  string.cpp
)

set_source_files_properties(
  dll.hpp.in

  PROPERTIES
  HEADER_FILE_ONLY ON)

if(BUILD_SHARED_LIBS)
  list(APPEND dmitigr_internal_headers ${CMAKE_CURRENT_SOURCE_DIR}/dll.hpp)
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

  add_library(${dmitigr_internal_target} ${dmitigr_internal_headers} ${dmitigr_internal_implementations})

  if(BUILD_SHARED_LIBS)
    target_compile_definitions(dmitigr_internal PUBLIC -DDMITIGR_INTERNAL_DLL)
  endif()

  set_target_properties(${dmitigr_internal_target}
    PROPERTIES
    LINKER_LANGUAGE "CXX"
    POSITION_INDEPENDENT_CODE True
    VERSION ${INTERNAL_VERSION_PART1}.${INTERNAL_VERSION_PART2}
    DEBUG_POSTFIX "d")

  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      # FIXME: GDB 7.7 doesn't print info of rvalues' that are function arguments
      # properly when compiled with -gdwarf-4. So, using -gdwarf-3 instead.
      target_compile_options(${dmitigr_internal_target} PRIVATE
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
      target_compile_options(${dmitigr_internal_target} PRIVATE
        -pedantic
        -Weverything)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
      target_compile_options(${dmitigr_internal_target} PRIVATE
        /Zc:throwingNew,referenceBinding,noexceptTypes
        /errorReport:none
        /nologo
        /utf-8)
    endif()
  endif()

  if (UNIX)
    target_link_libraries(${dmitigr_internal_target} stdc++fs)
  elseif (WIN32)
    target_link_libraries(${dmitigr_internal_target} Ws2_32.lib)
  endif()
else()
  set(dmitigr_internal_target "dmitigr_internal_interface")

  add_library(${dmitigr_internal_target} INTERFACE)

  target_compile_definitions(${dmitigr_internal_target} INTERFACE -DDMITIGR_INTERNAL_HEADER_ONLY)

  if (UNIX)
    target_link_libraries(${dmitigr_internal_target} INTERFACE stdc++fs)
  elseif (WIN32)
    target_link_libraries(${dmitigr_internal_target} INTERFACE Ws2_32.lib)
  endif()
endif()

# ------------------------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------------------------

set(CMAKE_MODULE_PATH ${dmitigr_internal_SOURCE_DIR}/cmake)

# GraphicsMagick
find_package(GraphicsMagick)
if (GraphicsMagick_FOUND)
  message("Found GraphicsMagick++ headers at: ${GraphicsMagick_INCLUDE_DIRS}")
  list(APPEND dmitigr_internal_headers graphicsmagick.hpp)
  list(APPEND dmitigr_internal_implementations graphicsmagick.cpp)
  include_directories(${GraphicsMagick_INCLUDE_DIRS})
  if(NOT DMITIGR_INTERNAL_HEADER_ONLY)
    target_link_libraries(${dmitigr_internal_target} ${Suggested_GraphicsMagick_LIBRARIES})
  else()
    target_link_libraries(${dmitigr_internal_target} INTERFACE ${Suggested_GraphicsMagick_LIBRARIES})
  endif()
endif()

# ------------------------------------------------------------------------------
# Installations
# ------------------------------------------------------------------------------

install(FILES ${dmitigr_internal_headers}
  DESTINATION ${INTERNAL_INCLUDES_INSTALL_DIR}/dmitigr/internal)

if(NOT DMITIGR_INTERNAL_HEADER_ONLY)
  install(TARGETS ${dmitigr_internal_target}
    EXPORT ${dmitigr_internal_target}_export
    ARCHIVE  DESTINATION ${INTERNAL_LIBRARY_INSTALL_DIR}
    LIBRARY  DESTINATION ${INTERNAL_LIBRARY_INSTALL_DIR}
    RUNTIME  DESTINATION ${INTERNAL_LIBRARY_INSTALL_DIR}
    INCLUDES DESTINATION ${INTERNAL_INCLUDES_INSTALL_DIR})
else()
  install(TARGETS ${dmitigr_internal_target}
    EXPORT ${dmitigr_internal_target}_export
    INCLUDES DESTINATION ${INTERNAL_INCLUDES_INSTALL_DIR})

  install(FILES ${dmitigr_internal_implementations}
    DESTINATION ${INTERNAL_INCLUDES_INSTALL_DIR}/dmitigr/internal)
endif()

install(EXPORT ${dmitigr_internal_target}_export
  DESTINATION ${INTERNAL_CMAKE_INSTALL_DIR}
  FILE ${dmitigr_internal_target}-config.cmake)
