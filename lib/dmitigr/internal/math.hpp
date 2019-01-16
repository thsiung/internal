// -*- C++ -*-
// Copyright (C) Dmitry Igrishin
// For conditions of distribution and use, see files LICENSE.txt or internal.hpp

#ifndef DMITIGR_INTERNAL_MATH_HPP
#define DMITIGR_INTERNAL_MATH_HPP

#include <cstdlib>

namespace dmitigr::internal::math {

/**
 * @internal
 *
 * @return The random number.
 *
 * @remarks From TC++PL 3rd, 22.7.
 */
template<typename T>
T rand_cpp_pl_3rd(const T& num)
{
  return T(double(std::rand()) / RAND_MAX) * num;
}

} // namespace dmitigr::internal::math

#endif  // DMITIGR_INTERNAL_MATH_HPP
