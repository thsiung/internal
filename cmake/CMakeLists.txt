# -*- cmake -*-
# Copyright (C) Dmitry Igrishin
# For conditions of distribution and use, see files LICENSE.txt or internal.hpp

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message("")
  message("Starting the debug output of testing librarian.cmake:")
  foreach(lib "GraphicsMagick" "Pq")
    find_package(${lib})
    include(librarian_debug)
  endforeach()
  message("Ending the debug output of testing librarian.cmake.")
  message("")
endif()
