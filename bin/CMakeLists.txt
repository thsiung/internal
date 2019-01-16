# -*- cmake -*-
# Copyright (C) Dmitry Igrishin
# For conditions of distribution and use, see files LICENSE.txt or internal.hpp

install(PROGRAMS
  dmitigr_internal_adapt

  DESTINATION bin)

install(FILES
  dmitigr_internal_basics.sh

  DESTINATION bin)
