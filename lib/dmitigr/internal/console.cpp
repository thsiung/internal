// -*- C++ -*-
// Copyright (C) Dmitry Igrishin
// For conditions of distribution and use, see files LICENSE.txt or internal.hpp

#include "dmitigr/internal/implementation_header.hpp"

#include "dmitigr/internal/console.hpp"
#include "dmitigr/internal/debug.hpp"

#include <stdexcept>

namespace dmitigr::internal::console {

DMITIGR_INTERNAL_INLINE void Command::throw_invalid_usage(std::string details) const
{
  std::string message = "invalid usage of the \"" + name() + "\" command\n";
  if (!details.empty())
    message.append("  details: " + std::move(details) + "\n");
  throw std::logic_error{std::move(message.append(usage()))};
}

DMITIGR_INTERNAL_INLINE std::optional<std::string> Command::option_argument(const std::string& value, const bool is_optional) const
{
  if (const auto pos = value.find('='); pos != std::string::npos)
    return pos < value.size() ? value.substr(pos + 1) : std::string{};
  else if (is_optional)
    return std::nullopt;
  else
    throw_invalid_usage("no argument for the \"" + value.substr(0, pos) + "\" option specified");
}

DMITIGR_INTERNAL_INLINE void Command::check_no_option_argument(const std::string& value) const
{
  DMITIGR_INTERNAL_ASSERT(value.find("--") == 0);
  if (const auto pos = value.find('='); pos != std::string::npos)
    throw_invalid_usage("no argument for the option \"" + value.substr(0, pos) + "\" can be specified");
}

/**
 * @brief Parses the options.
 *
 * @param i - The starting iterator of the options to parse.
 * @param e - The ending iterator of the options to parse.
 * @param parse_option - The callback that will be called for each option.
 * The parser must accepts one argument: the string of the option to parse.
 */
DMITIGR_INTERNAL_INLINE auto Command::parse_options(Option_iterator i, const Option_iterator e, Option_parser parse_option) -> Option_iterator
{
  for (; i != e && *i != "--" && (i->find("--") == 0); ++i)
    parse_option(*i);
  return i;
}

DMITIGR_INTERNAL_INLINE std::pair<std::string, std::vector<std::string>> command_and_options(const int argc, const char* const* argv)
{
  DMITIGR_INTERNAL_ASSERT(argc > 1);
  std::string result1{argv[1]};
  std::vector<std::string> result2;
  for (int i = 2; i < argc; ++i)
    result2.push_back(argv[i]);
  return std::make_pair(result1, result2);
}

} // namespace dmitigr::internal::console

#include "dmitigr/internal/implementation_footer.hpp"
