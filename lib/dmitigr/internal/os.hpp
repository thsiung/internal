// -*- C++ -*-
// Copyright (C) Dmitry Igrishin
// For conditions of distribution and use, see files LICENSE.txt or internal.hpp

#ifndef DMITIGR_INTERNAL_OS_HPP
#define DMITIGR_INTERNAL_OS_HPP
#include "dmitigr/internal/dll.hpp"

#include <cstddef>
#include <optional>
#include <string>

namespace dmitigr::internal::os {

/**
 * @internal
 *
 * @returns The current working directory.
 */
DMITIGR_INTERNAL_API std::string current_working_directory();

/**
 * @internal
 *
 * @returns The string with the current username.
 */
DMITIGR_INTERNAL_API std::string current_username();

/**
 * @internal
 *
 * @returns The value of the environment variable `name`, or std::nullopt if there is no match.
 *
 * @remarks Cannot be used in applications that execute in the Windows Runtime, because
 * environment variables are not available to UWP applications.
 */
DMITIGR_INTERNAL_API std::optional<std::string> environment_variable(const std::string& name);

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

DMITIGR_INTERNAL_API std::size_t seek(int fd, long offset, Origin whence);

DMITIGR_INTERNAL_API std::size_t read(int fd, void* buffer, unsigned int count);

} // namespace io

} // namespace dmitigr::internal::os

#ifdef DMITIGR_INTERNAL_HEADER_ONLY
#include "dmitigr/internal/os.cpp"
#endif

#endif  // DMITIGR_INTERNAL_OS_HPP
