// -*- C++ -*-
// Copyright (C) Dmitry Igrishin
// For conditions of distribution and use, see files LICENSE.txt or internal.hpp

#ifndef DMITIGR_INTERNAL_GRAPHICSMAGICK_HPP
#define DMITIGR_INTERNAL_GRAPHICSMAGICK_HPP
#include "dmitigr/internal/dll.hpp"

#include <iosfwd>

namespace dmitigr::internal::img::graphicsmagick {

enum class File_type { gif = 1, jpeg, png };

/**
 * @internal
 *
 * @brief GraphicsMagick initialization.
 */
DMITIGR_INTERNAL_API void init(const char* app_path);

/**
 * @internal
 *
 * @returns MIME in the GraphicsMagick notation.
 */
DMITIGR_INTERNAL_API const char* file_type_c_str(File_type file_type);

/**
 * @internal
 *
 * @brief Resizes the image read from `input` and writes the result to `output`.
 */
DMITIGR_INTERNAL_API void resize(std::istream& input,
  std::ostream& output,
  unsigned int output_x,
  unsigned int output_y,
  File_type output_type,
  unsigned int output_quality = 90);

} // namespace dmitigr::internal::img::graphicsmagick

#ifdef DMITIGR_INTERNAL_HEADER_ONLY
#include "dmitigr/internal/graphicsmagick.cpp"
#endif

#endif  // DMITIGR_INTERNAL_GRAPHICSMAGICK_HPP
