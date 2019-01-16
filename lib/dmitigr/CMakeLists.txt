# -*- cmake -*-
# Copyright (C) Dmitry Igrishin
# For conditions of distribution and use, see files LICENSE.txt or internal.hpp

install(FILES
  internal.hpp

  DESTINATION ${INTERNAL_INCLUDES_INSTALL_DIR}/dmitigr)

# ------------------------------------------------------------------------------

add_subdirectory(internal)
