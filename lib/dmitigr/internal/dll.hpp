// -*- C++ -*-
// Copyright (C) Dmitry Igrishin
// For conditions of distribution and use, see files LICENSE.txt or internal.hpp

#ifndef DMITIGR_INTERNAL_DLL_HPP
#define DMITIGR_INTERNAL_DLL_HPP

/*
 * This file must be adapted for every API individually!
 */

#ifdef _WIN32
  #ifdef DMITIGR_INTERNAL_DLL_BUILDING
    #define DMITIGR_INTERNAL_API __declspec(dllexport)
  #else
    #if DMITIGR_INTERNAL_DLL
      #define DMITIGR_INTERNAL_API __declspec(dllimport)
    #else /* static library on Windows */
      #define DMITIGR_INTERNAL_API
    #endif
  #endif
#else /* Unix */
  #define DMITIGR_INTERNAL_API
#endif

#endif // DMITIGR_INTERNAL_DLL_HPP