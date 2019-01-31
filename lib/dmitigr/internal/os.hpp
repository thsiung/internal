// -*- C++ -*-
// Copyright (C) Dmitry Igrishin
// For conditions of distribution and use, see files LICENSE.txt or internal.hpp

#ifndef DMITIGR_INTERNAL_OS_HPP
#define DMITIGR_INTERNAL_OS_HPP

#include <cstddef>
#include <string>

namespace dmitigr::internal::os {

/**
 * @internal
 *
 * @returns The current working directory.
 */
std::string cwd();

/**
 * @internal
 *
 * @returns The string with the current username.
 */
std::string current_username();

// -----------------------------------------------------------------------------

namespace io {

enum Origin {
  seek_set = SEEK_SET,
  seek_cur = SEEK_CUR,
  seek_end = SEEK_END,
#ifndef _WIN32
  seek_data = SEEK_DATA,
  seek_hole = SEEK_HOLE
#endif
};

std::size_t seek(int fd, long offset, Origin whence);

std::size_t read(int fd, void* buffer, unsigned int count);

} // namespace io

} // namespace dmitigr::internal::os

#ifdef DMITIGR_INTERNAL_HEADER_ONLY
#include "dmitigr/internal/os.cpp"
#endif

#endif  // DMITIGR_INTERNAL_OS_HPP
