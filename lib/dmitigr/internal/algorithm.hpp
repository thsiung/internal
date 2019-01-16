// -*- C++ -*-
// Copyright (C) Dmitry Igrishin
// For conditions of distribution and use, see files LICENSE.txt or internal.hpp

#ifndef DMITIGR_INTERNAL_ALGORITHM_HPP
#define DMITIGR_INTERNAL_ALGORITHM_HPP

#include <algorithm>

namespace dmitigr::internal::algorithm {

/**
 * @internal
 *
 * @brief Remove duplicates from the given container.
 */
template<class Container>
void eliminate_duplicates(Container& cont)
{
  std::sort(begin(cont), end(cont));
  cont.erase(std::unique(begin(cont), end(cont)), end(cont));
}

/**
 * @internal
 *
 * @returns `true` if input begins with pattern.
 */
template<class Container>
bool is_begins_with(const Container& input, const Container& pattern)
{
  return (pattern.size() <= input.size()) &&
    std::equal(cbegin(input), cend(input), cbegin(pattern));
}

} // namespace dmitigr::internal::algorithm

#endif  // DMITIGR_INTERNAL_ALGORITHM_HPP
