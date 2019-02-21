// -*- C++ -*-
// Copyright (C) Dmitry Igrishin
// For conditions of distribution and use, see files LICENSE.txt or internal.hpp

#ifndef DMITIGR_INTERNAL_INLINE
  #ifdef DMITIGR_INTERNAL_HEADER_ONLY
    #define DMITIGR_INTERNAL_INLINE inline
  #else
    #define DMITIGR_INTERNAL_INLINE
  #endif
#endif  // DMITIGR_INTERNAL_INLINE

#ifndef DMITIGR_INTERNAL_NOMINMAX
  #ifdef _WIN32
    #ifndef NOMINMAX
      #define NOMINMAX
      #define DMITIGR_INTERNAL_NOMINMAX
    #endif
  #endif
#endif  // DMITIGR_INTERNAL_NOMINMAX
