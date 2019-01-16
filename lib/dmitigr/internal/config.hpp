// -*- C++ -*-
// Copyright (C) Dmitry Igrishin
// For conditions of distribution and use, see files LICENSE.txt or internal.hpp

#ifndef DMITIGR_INTERNAL_CONFIG_HPP
#define DMITIGR_INTERNAL_CONFIG_HPP

#include "dmitigr/internal/filesystem.hpp"

#include <map>
#include <optional>
#include <string>
#include <utility>

namespace dmitigr::internal::config {

/**
 * @brief Class Flat represents a flat configuration store.
 */
class Flat {
public:
  explicit Flat(const std::filesystem::path& path);

  const std::optional<std::string>& string_parameter(const std::string& name) const;

  std::optional<bool> boolean_parameter(const std::string& name) const;

  const std::map<std::string, std::optional<std::string>>& parameters() const;

private:
  /**
   * @returns Parsed config entry. The format of `line` can be:
   *   - "param=one";
   *   - "param='one two  three';
   *   - "param='one \'two three\' four'.
   */
  std::pair<std::string, std::string> parsed_config_entry(const std::string& line);

  /**
   * @returns Parsed config file. The format of `line` can be:
   *   - "param=one";
   *   - "param='one two  three';
   *   - "param='one \'two three\' four'.
   */
  std::map<std::string, std::optional<std::string>> parsed_config(const std::filesystem::path& path);

  bool is_invariant_ok() const;

  static const std::optional<std::string>& null_string_parameter();

  std::map<std::string, std::optional<std::string>> parameters_;
};

} // namespace dmitigr::internal::config

#endif  // DMITIGR_INTERNAL_CONFIG_HPP
