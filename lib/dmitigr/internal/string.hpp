// -*- C++ -*-
// Copyright (C) Dmitry Igrishin
// For conditions of distribution and use, see files LICENSE.txt or internal.hpp

#ifndef DMITIGR_INTERNAL_STRING_HPP
#define DMITIGR_INTERNAL_STRING_HPP
#include "dmitigr/internal/dll.hpp"

#include "dmitigr/internal/debug.hpp"

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <functional>
#include <initializer_list>
#include <limits>
#include <locale>
#include <string>
#include <utility>

namespace dmitigr::internal::string {

// -----------------------------------------------------------------------------
// C strings

/**
 * @internal
 *
 * @returns The pointer to the next non-space character, or pointer to the
 * terminating zero character.
 */
DMITIGR_INTERNAL_API const char* next_non_space_pointer(const char* p, const std::locale& loc = {}) noexcept;

/**
 * @internal
 *
 * @returns The specified literal if `(literal != nullptr)`, or "" otherwise.
 */
inline const char* literal(const char* const literal) noexcept
{
  return literal ? literal : "";
}

/**
 * @internal
 *
 * @returns First non-null string literal of specified literals, or
 * `nullptr` if there is no such a literal.
 */
DMITIGR_INTERNAL_API const char* coalesce(std::initializer_list<const char*> literals) noexcept;

// -----------------------------------------------------------------------------
// Text lines manipulations

/**
 * @internal
 *
 * @returns Line number by the given absolute position. (Line numbers starts at 1.)
 */
DMITIGR_INTERNAL_API std::size_t line_number_by_position(const std::string& str, const std::size_t pos);

/**
 * @internal
 *
 * @returns Line and column numbers by the given absolute position. (Both numbers starts at 1.)
 */
DMITIGR_INTERNAL_API
std::pair<std::size_t, std::size_t> line_column_numbers_by_position(const std::string& str, const std::size_t pos);

// -----------------------------------------------------------------------------
// Predicates

/**
 * @internal
 *
 * @returns `true` if `c` is a valid space character.
 */
inline bool is_space_character(const char c, const std::locale& loc = {})
{
  return std::isspace(c, loc);
}

/**
 * @internal
 *
 * @returns !is_space_character(c);
 */
inline bool is_non_space_character(const char c, const std::locale& loc = {})
{
  return !is_space_character(c, loc);
}

/**
 * @internal
 *
 * @returns `true` if `c` is a valid simple identifier character.
 */
inline bool is_simple_identifier_character(const char c, const std::locale& loc = {})
{
  return std::isalnum(c, loc) || c == '_';
}

/**
 * @internal
 *
 * @returns !is_simple_identifier_character(c).
 */
inline bool is_non_simple_identifier_character(const char c, const std::locale& loc = {})
{
  return !is_simple_identifier_character(c, loc);
}

/**
 * @internal
 *
 * @returns `true` if `str` has at least one space character.
 */
inline bool has_space(const std::string& str, const std::locale& loc = {})
{
  using namespace std::placeholders;
  return std::any_of(cbegin(str), cend(str), std::bind(is_space_character, _1, loc));
}

/**
 * @internal
 *
 * @returns `true` if `input` is starting with `pattern`.
 */
bool is_begins_with(std::string_view input, std::string_view pattern);

// -----------------------------------------------------------------------------
// Generators

/**
 * @internal
 *
 * @returns A random string of specified size from chars of palette.
 */
DMITIGR_INTERNAL_API std::string random_string(const std::string& palette, std::string::size_type size);

/**
 * @internal
 *
 * @returns A random string of specified size from chars in the range [beg,end).
 *
 * @par Requires
 * `(beg < end)`
 */
DMITIGR_INTERNAL_API std::string random_string(char beg, char end, std::string::size_type size);

// -----------------------------------------------------------------------------
// Transformators

/**
 * @internal
 *
 * @returns "i_n_p_u_t", where the "_" is the value of the `separator`.
 */
DMITIGR_INTERNAL_API std::string sparsed_string(const std::string& input, const std::string& separator);

/**
 * @internal
 *
 * @par Effects
 * `(str.back() == c)`
 */
DMITIGR_INTERNAL_API void terminate_string(std::string& str, char c);

/**
 * @internal
 *
 * Replaces all uppercase characters in `str` by the corresponding lowercase characters.
 */
DMITIGR_INTERNAL_API void lowercase(std::string& str, const std::locale& loc = {});

/**
 * @internal
 *
 * @returns The modified copy of the `str` with all uppercase characters replaced by the
 * corresponding lowercase characters.
 */
DMITIGR_INTERNAL_API std::string to_lowercase(const std::string& str, const std::locale& loc = {});

/**
 * @internal
 *
 * Replaces all lowercase characters in `str` by the corresponding uppercase characters.
 */
DMITIGR_INTERNAL_API void uppercase(std::string& str, const std::locale& loc = {});

/**
 * @internal
 *
 * @returns The modified copy of the `str` with all lowercase characters replaced by the
 * corresponding uppercase characters.
 */
DMITIGR_INTERNAL_API std::string to_uppercase(const std::string& str, const std::locale& loc = {});

/**
 * @internal
 *
 * @returns `true` if all of character of `str` are in uppercase, or `false` otherwise.
 */
DMITIGR_INTERNAL_API bool is_lowercased(std::string_view str, const std::locale& loc = {});

/**
 * @internal
 *
 * @returns `true` if all of character of `str` are in lowercase, or `false` otherwise.
 */
DMITIGR_INTERNAL_API bool is_uppercased(std::string_view str, const std::locale& loc = {});

// -----------------------------------------------------------------------------
// Substrings

/**
 * @returns The position of the first non-space character of `str` in range [pos, str.size()).
 */
DMITIGR_INTERNAL_API std::string::size_type position_of_non_space(const std::string& str,
  std::string::size_type pos, const std::locale& loc = {});

/**
 * @returns The substring of `str` from position of `pos` until the position
 * of the character "c" that `pred(c) == false` as the first element, and the
 * position of the character followed "c" as the second element.
 */
template<typename Pred>
std::pair<std::string, std::string::size_type> substring_if(const std::string& str, Pred pred,
  std::string::size_type pos, const std::locale& loc = {})
{
  DMITIGR_INTERNAL_ASSERT(pos <= str.size());
  std::pair<std::string, std::string::size_type> result;
  const auto input_size = str.size();
  for (; pos < input_size; ++pos) {
    if (pred(str[pos], loc))
      result.first += str[pos];
    else
      break;
  }
  result.second = pos;
  return result;
}

/**
 * @returns The substring of `str` with the "simple identifier" from position of `pos`
 * as the first element, and the position of the next character in `str`.
 */
DMITIGR_INTERNAL_API
std::pair<std::string, std::string::size_type> substring_if_simple_identifier(const std::string& str,
  std::string::size_type pos, const std::locale& loc = {});

/**
 * @returns The substring of `str` with no spaces from position of `pos`
 * as the first element, and the position of the next character in `str`.
 */
DMITIGR_INTERNAL_API
std::pair<std::string, std::string::size_type> substring_if_no_spaces(const std::string& str,
  std::string::size_type pos, const std::locale& loc = {});

/**
 * @returns The unquoted substring of `str` if `str[pos] == '\''` or the substring
 * with no spaces from the position of `pos` as the first element, and the position
 * of the next character in `str`.
 */
DMITIGR_INTERNAL_API
std::pair<std::string, std::string::size_type> unquoted_substring(const std::string& str,
  std::string::size_type pos, const std::locale& loc = {});

// -----------------------------------------------------------------------------
// Sequence converters

/**
 * @internal
 *
 * @returns The string with stringified elements of the sequence in range [b, e).
 */
template<class InputIterator, typename Function>
std::string to_string(const InputIterator b, const InputIterator e, const std::string& sep, Function to_str)
{
  std::string result;
  if (b != e) {
    auto i = b;
    for (; i != e; ++i) {
      result.append(to_str(*i));
      result.append(sep);
    }
    const std::string::size_type sep_size = sep.size();
    for (std::string::size_type i = 0; i < sep_size; ++i)
      result.pop_back();
  }
  return result;
}

/**
 * @internal
 *
 * @returns The string with stringified elements of the Container.
 */
template<class Container, typename Function>
std::string to_string(const Container& cont, const std::string& sep, Function to_str)
{
  return to_string(cbegin(cont), cend(cont), sep, to_str);
}

/**
 * @internal
 *
 * @returns The string with stringified elements of the Container.
 */
template<class Container>
std::string to_string(const Container& cont, const std::string& sep)
{
  return to_string(cont, sep, [](const std::string& e)->const auto& { return e; });
}

// -----------------------------------------------------------------------------
// Numeric converters

/**
 * @internal
 *
 * @returns The string object holding the character representation of the `value`
 * according to the given `base`.
 *
 * @par Requires
 * `(2 <= base && base <= 36)`
 */
template<typename Number>
std::enable_if_t<std::is_integral<Number>::value, std::string>
to_string(Number value, const Number base = 10)
{
  DMITIGR_INTERNAL_ASSERT(2 <= base && base <= 36);
  static_assert(std::numeric_limits<Number>::min() <= 2 && std::numeric_limits<Number>::max() >= 36, "");
  static const char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                                'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
                                'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
                                'U', 'V', 'W', 'X', 'Y', 'Z'};
  static_assert(sizeof(digits) == 36, "");
  const bool negative = (value < 0);
  std::string result;
  if (negative)
    value = -value;
  while (value >= base) {
    const auto rem = value % base;
    value /= base;
    result += digits[rem];
  }
  result += digits[value];
  if (negative)
    result += '-';
  std::reverse(begin(result), end(result));
  return result;
}

} // namespace dmitigr::internal::string

#ifdef DMITIGR_INTERNAL_HEADER_ONLY
#include "dmitigr/internal/string.cpp"
#endif

#endif  // DMITIGR_INTERNAL_STRING_HPP
