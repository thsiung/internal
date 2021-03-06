// -*- C++ -*-
// Copyright (C) Dmitry Igrishin
// For conditions of distribution and use, see files LICENSE.txt or internal.hpp

#include "dmitigr/internal/debug.hpp"
#include "dmitigr/internal/stream.hpp"

#include <istream>
#include <locale>

#include "dmitigr/internal/implementation_header.hpp"

namespace dmitigr::internal::stream {

DMITIGR_INTERNAL_INLINE Read_exception::Read_exception(const std::error_condition condition)
  : system_error{condition.value(), error_category()}
{}

DMITIGR_INTERNAL_INLINE Read_exception::Read_exception(std::error_condition condition, std::string&& incomplete_result)
  : system_error{condition.value(), error_category()}
  , incomplete_result_{std::move(incomplete_result)}
{}

DMITIGR_INTERNAL_INLINE const std::string& Read_exception::incomplete_result() const
{
  return incomplete_result_;
}

DMITIGR_INTERNAL_INLINE const char* Read_exception::what() const noexcept
{
  return "dmitigr::internal::stream::Read_exception";
}

// -----------------------------------------------------------------------------

DMITIGR_INTERNAL_INLINE const char* Error_category::name() const noexcept
{
  return "dmitigr_internal_stream_error";
}

DMITIGR_INTERNAL_INLINE std::string Error_category::message(const int ev) const
{
  return "dmitigr_internal_stream_error " + std::to_string(ev);
}

// -----------------------------------------------------------------------------

DMITIGR_INTERNAL_INLINE auto error_category() noexcept -> const Error_category&
{
  static Error_category result;
  return result;
}

DMITIGR_INTERNAL_INLINE std::error_code make_error_code(Read_errc errc) noexcept
{
  return std::error_code(int(errc), error_category());
}

DMITIGR_INTERNAL_INLINE std::error_condition make_error_condition(Read_errc errc) noexcept
{
  return std::error_condition(int(errc), error_category());
}

// -----------------------------------------------------------------------------

DMITIGR_INTERNAL_INLINE std::string read_to_string(std::istream& input)
{
  constexpr std::size_t buffer_size{512};
  std::string result;
  char buffer[buffer_size];
  while (input.read(buffer, buffer_size))
    result.append(buffer, buffer_size);
  result.append(buffer, std::size_t(input.gcount()));
  return result;
}

DMITIGR_INTERNAL_INLINE std::string read_simple_phrase_to_string(std::istream& input)
{
  std::string result;

  const auto check_input_state =
    [&]()
    {
      if (input.fail() && !input.eof())
        throw Read_exception{Read_errc::stream_error, std::move(result)};
    };

  // Skip whitespaces (i.e. ' ', '\t' and '\n').
  char ch;
  while (input.get(ch) && std::isspace(ch, std::locale{}));
  check_input_state();

  if (input) {
    if (ch == '"') {
      // Try to reach the trailing quote character.
      const char quote_char = ch;
      constexpr char escape_char = '\\';
      while (input.get(ch) && ch != quote_char) {
        if (ch == escape_char) {
          // Escape character were reached. Read the next character to analyze.
          if (input.get(ch)) {
            if (ch != quote_char)
              /*
               * The "escape" character does not really escape anything,
               * thus must be preserved into the result string.
               */
              result += escape_char;
            result += ch;
          }
        } else
          result += ch;
      }
      check_input_state();

      if (ch != quote_char) {
        // The trailing quote character was NOT reached.
        DMITIGR_INTERNAL_ASSERT(input.eof());
        throw Read_exception{Read_errc::invalid_input, std::move(result)};
      }
    } else {
      /*
       * There is no leading quote detected.
       * So read characters until EOF, space, newline or the quote.
       */
      result += ch;
      while (input.get(ch) && !std::isspace(ch, std::locale{}) && ch != '"')
        result += ch;
      check_input_state();
      if (std::isspace(ch, std::locale{}) || ch == '"')
        input.putback(ch);
    }
  }

  return result;
}

} // namespace dmitigr::internal::stream

#include "dmitigr/internal/implementation_footer.hpp"
