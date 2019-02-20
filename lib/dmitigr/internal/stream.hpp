// -*- C++ -*-
// Copyright (C) Dmitry Igrishin
// For conditions of distribution and use, see files LICENSE.txt or internal.hpp

#ifndef DMITIGR_INTERNAL_STREAM_HPP
#define DMITIGR_INTERNAL_STREAM_HPP
#include "dmitigr/internal/dll.hpp"

#include <iosfwd>
#include <string>
#include <system_error>

// Exceptions and error codes
namespace dmitigr::internal::stream {

/**
 * @brief Represents an exception that may be thrown by `read_*()` functions.
 */
class Read_exception : public std::system_error {
public:
  DMITIGR_INTERNAL_API explicit Read_exception(std::error_condition condition);

  DMITIGR_INTERNAL_API Read_exception(std::error_condition condition, std::string&& incomplete_result);

  DMITIGR_INTERNAL_API const std::string& incomplete_result() const;

  DMITIGR_INTERNAL_API const char* what() const noexcept override;

private:
  std::string incomplete_result_;
};

/**
 * @internal
 *
 * @brief Represents a read error code.
 */
enum class Read_errc { success = 0, stream_error, invalid_input };

/**
 * @internal
 *
 * @brief Represents a type derived from class `std::error_category` to support category
 * of dmitigr::internal::parse runtime errors.
 */
class Error_category : public std::error_category {
public:
  DMITIGR_INTERNAL_API const char* name() const noexcept override;

  DMITIGR_INTERNAL_API std::string message(const int ev) const override;
};

/**
 * @internal
 *
 * @returns A reference to an object of a type derived from class `std::error_category`.
 *
 * @remarks The object's name() function returns a pointer to the string "dmitigr_internal_stream_error".
 */
DMITIGR_INTERNAL_API const Error_category& error_category() noexcept;

/**
 * @internal
 *
 * @returns `std::error_code(int(errc), parse_error_category())`
 */
DMITIGR_INTERNAL_API std::error_code make_error_code(Read_errc errc) noexcept;

/**
 * @internal
 *
 * @returns `std::error_condition(int(errc), parse_error_category())`
 */
DMITIGR_INTERNAL_API std::error_condition make_error_condition(Read_errc errc) noexcept;

} // namespace dmitigr::internal::stream

// Integration with the std::system_error framework
namespace std {

template<> struct is_error_condition_enum<dmitigr::internal::stream::Read_errc> : true_type {};

} // namespace std

namespace dmitigr::internal::stream {

/**
 * @internal
 *
 * @brief Reads a whole stream to a string.
 *
 * @returns The string with the content read from the stream.
 */
DMITIGR_INTERNAL_API std::string read_to_string(std::istream& input);

/**
 * @internal
 *
 * @brief Reads the (next) "simple phrase" from the `input`.
 *
 * Whitespaces (i.e. space, tab or newline) or the quote (i.e. '"') that follows
 * after the phrase are preserved in the `input`.
 *
 * @returns The string with the "simple phrase".
 *
 * @throws Simple_phrase_error with the appropriate code and incomplete result
 * of parsing.
 *
 * @remarks the "simple phrase" - an unquoted expression without spaces, or
 * quoted expression (which can include any characters).
 */
DMITIGR_INTERNAL_API std::string read_simple_phrase_to_string(std::istream& input);

} // namespace dmitigr::internal::stream

#ifdef DMITIGR_INTERNAL_HEADER_ONLY
#include "dmitigr/internal/stream.cpp"
#endif

#endif  // DMITIGR_INTERNAL_STREAM_HPP
