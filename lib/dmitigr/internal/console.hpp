// -*- C++ -*-
// Copyright (C) Dmitry Igrishin
// For conditions of distribution and use, see files LICENSE.txt or internal.hpp

#ifndef DMITIGR_INTERNAL_CONSOLE_HPP
#define DMITIGR_INTERNAL_CONSOLE_HPP

#include "dmitigr/internal/dll.hpp"

#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace dmitigr::internal::console {

/**
 * @internal
 *
 * @brief Represents an application's command to run.
 */
class Command {
public:
  /** The destructor */
  virtual ~Command() = default;

  /** @returns The name of the command. */
  virtual std::string name() const = 0;

  /** @return The usage string of the command. */
  virtual std::string usage() const = 0;

  /** Runs the command. */
  virtual void run() = 0;

protected:
  /** Represents a vector of command options. */
  using Option_vector = std::vector<std::string>;

  /** Represents an iterator of command options. */
  using Option_iterator = typename Option_vector::const_iterator;

  /** Represents a setter of command options. */
  using Option_parser = std::function<void (const std::string&)>;

  /**
   * @throws An instance of std::logic_error.
   *
   * @param details - The details to be included into the error message.
   */
  [[noreturn]] DMITIGR_INTERNAL_API void throw_invalid_usage(std::string details = {}) const;

  /**
   * @returns The argument that follows the option.
   *
   * @param value - The value the option.
   * @param is_optional - The indicator of an optional option argument.
   */
  DMITIGR_INTERNAL_API std::optional<std::string> option_argument(const std::string& value, const bool is_optional = false) const;

  DMITIGR_INTERNAL_API void check_no_option_argument(const std::string& value) const;

  /**
   * @brief Parses the options.
   *
   * @param i - The starting iterator of the options to parse.
   * @param e - The ending iterator of the options to parse.
   * @param parse_option - The callback that will be called for each option.
   * The parser must accepts one argument: the string of the option to parse.
   */
  DMITIGR_INTERNAL_API Option_iterator parse_options(Option_iterator i, const Option_iterator e, Option_parser parse_option);
};

/**
 * @returns The command ID and options.
 *
 * The command ID - is an identifier specified as the 1st argument.
 * For example, the "exec" is the command ID here:
 *   pgspa exec --strong foo bar baz
 */
DMITIGR_INTERNAL_API std::pair<std::string, std::vector<std::string>> command_and_options(const int argc, const char* const* argv);

} // namespace dmitigr::internal::console

#ifdef DMITIGR_INTERNAL_HEADER_ONLY
#include "dmitigr/internal/console.cpp"
#endif

#endif // DMITIGR_INTERNAL_CONSOLE_HPP
