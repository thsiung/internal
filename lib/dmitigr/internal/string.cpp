// -*- C++ -*-
// Copyright (C) Dmitry Igrishin
// For conditions of distribution and use, see files LICENSE.txt or internal.hpp

#include "dmitigr/internal/header_only.hpp"

#include "dmitigr/internal/math.hpp"
#include "dmitigr/internal/string.hpp"

#include <type_traits>

namespace str = dmitigr::internal::string;

DMITIGR_INLINE const char* str::next_non_space_pointer(const char* p, const std::locale& loc) noexcept
{
  if (p)
    while (*p != '\0' && std::isspace(*p, loc))
      ++p;
  return p;
}

DMITIGR_INLINE const char* str::coalesce(std::initializer_list<const char*> literals) noexcept
{
  for (const auto l : literals)
    if (l)
      return l;
  return nullptr;
}

// -----------------------------------------------------------------------------

DMITIGR_INLINE std::size_t str::line_number_by_position(const std::string& str, const std::size_t pos)
{
  DMITIGR_INTERNAL_ASSERT(pos < str.size());
  return std::count(cbegin(str), cbegin(str) + pos, '\n') + 1;
}

DMITIGR_INLINE std::pair<std::size_t, std::size_t>
str::line_column_numbers_by_position(const std::string& str, const std::size_t pos)
{
  DMITIGR_INTERNAL_ASSERT(pos < str.size());
  std::size_t line{}, column{};
  for (std::size_t i = 0; i < pos; ++i) {
    ++column;
    if (str[i] == '\n') {
      ++line;
      column = 0;
    }
  }
  return std::make_pair(line + 1, column + 1);
}

// -----------------------------------------------------------------------------

DMITIGR_INLINE bool str::is_begins_with(std::string_view input, std::string_view pattern)
{
  return (pattern.size() <= input.size()) && std::equal(cbegin(input), cend(input), cbegin(pattern));
}

// -----------------------------------------------------------------------------

DMITIGR_INLINE std::string str::random_string(const std::string& palette, const std::string::size_type size)
{
  std::string result;
  result.resize(size);
  if (const auto pallete_size = palette.size()) {
    using Counter = std::remove_const_t<decltype (pallete_size)>;
    for (Counter i = 0; i < size; ++i)
      result[i] = palette[math::rand_cpp_pl_3rd(pallete_size)];
  }
  return result;
}

DMITIGR_INLINE std::string str::random_string(const char beg, const char end, const std::string::size_type size)
{
  DMITIGR_INTERNAL_ASSERT(beg < end);
  std::string result;
  result.resize(size);
  const auto length = end - beg;
  using Counter = std::remove_const_t<decltype (size)>;
  for (Counter i = 0; i < size; ++i) {
    result[i] = static_cast<char>((math::rand_cpp_pl_3rd(end) % length) + beg);
  }
  return result;
}

// -----------------------------------------------------------------------------

DMITIGR_INLINE std::string str::sparsed_string(const std::string& input, const std::string& separator)
{
  std::string result;
  if (!input.empty()) {
    result.reserve(input.size() + (input.size() - 1) * separator.size());
    auto i = begin(input);
    auto const e = end(input) - 1;
    for (; i != e; ++i) {
      result += *i;
      result += separator;
    }
    result += *i;
  }
  return result;
}

DMITIGR_INLINE void str::terminate_string(std::string& str, const char c)
{
  if (str.empty() || str.back() != c)
    str += c;
}

DMITIGR_INLINE void str::lowercase(std::string& str, const std::locale& loc)
{
  auto b = begin(str);
  auto e = end(str);
  std::transform(b, e, b, [&loc](const char c) { return std::tolower(c, loc); });
}

DMITIGR_INLINE std::string str::to_lowercase(const std::string& str, const std::locale& loc)
{
  std::string result{str};
  lowercase(result, loc);
  return result;
}

DMITIGR_INLINE void str::uppercase(std::string& str, const std::locale& loc)
{
  auto b = begin(str);
  auto e = end(str);
  std::transform(b, e, b, [&loc](const char c) { return std::toupper(c, loc); });
}

DMITIGR_INLINE std::string str::to_uppercase(const std::string& str, const std::locale& loc)
{
  std::string result{str};
  uppercase(result, loc);
  return result;
}

DMITIGR_INLINE bool str::is_lowercased(std::string_view str, const std::locale& loc)
{
  return std::all_of(cbegin(str), cend(str), [&loc](const char c) { return std::islower(c, loc); });
}

DMITIGR_INLINE bool str::is_uppercased(std::string_view str, const std::locale& loc)
{
  return std::all_of(cbegin(str), cend(str), [&loc](const char c) { return std::isupper(c, loc); });
}

// -----------------------------------------------------------------------------

DMITIGR_INLINE std::string::size_type str::position_of_non_space(const std::string& str,
  const std::string::size_type pos, const std::locale& loc)
{
  DMITIGR_INTERNAL_ASSERT(pos <= str.size());
  using namespace std::placeholders;
  const auto b = cbegin(str);
  return std::find_if(b + pos, cend(str), std::bind(is_non_space_character, _1, loc)) - b;
}

DMITIGR_INLINE std::pair<std::string, std::string::size_type>
str::substring_if_simple_identifier(const std::string& str,
  const std::string::size_type pos, const std::locale& loc)
{
  DMITIGR_INTERNAL_ASSERT(pos <= str.size());
  using namespace std::placeholders;
  return std::isalpha(str[pos], loc) ? substring_if(str, std::bind(is_simple_identifier_character, _1, loc), pos) :
    std::make_pair(std::string{}, pos);
}

DMITIGR_INLINE std::pair<std::string, std::string::size_type>
str::substring_if_no_spaces(const std::string& str, const std::string::size_type pos, const std::locale& loc)
{
  using namespace std::placeholders;
  return substring_if(str, std::bind(is_non_space_character, _1, loc), pos);
}
// -----------------------------------------------------------------------------

DMITIGR_INLINE std::pair<std::string, std::string::size_type>
str::unquoted_substring(const std::string& str, std::string::size_type pos, const std::locale& loc)
{
  DMITIGR_INTERNAL_ASSERT(pos <= str.size());
  using namespace std::placeholders;
  if (pos == str.size())
    return {std::string{}, pos};

  std::pair<std::string, std::string::size_type> result;
  constexpr char quote_char = '\'';
  constexpr char escape_char = '\\';
  if (str[pos] == quote_char) {
    // Trying to reach the trailing quote character.
    const auto input_size = str.size();
    enum { normal, escape } state = normal;
    for (++pos; pos < input_size; ++pos) {
      const auto ch = str[pos];
      switch (state) {
      case normal:
        if (ch == quote_char)
          goto finish;
        else if (ch == escape_char)
          state = escape;
        else
          result.first += ch;
        break;
      case escape:
        if (ch != quote_char)
          result.first += escape_char; // it's not escape, so preserve
        result.first += ch;
        state = normal;
        break;
      }
    }

  finish:
    if ((pos == input_size && str.back() != quote_char) || (pos < input_size && str[pos] != quote_char))
      throw std::runtime_error{"no trailing quote found"};
    else
      result.second = pos + 1; // discarding the trailing quote
  } else
    result = substring_if_no_spaces(str, pos, loc);
  return result;
}
