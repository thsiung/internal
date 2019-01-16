// -*- C++ -*-
// Copyright (C) Dmitry Igrishin
// For conditions of distribution and use, see files LICENSE.txt or internal.hpp

#ifndef DMITIGR_INTERNAL_NET_HPP
#define DMITIGR_INTERNAL_NET_HPP

#include <string>

namespace dmitigr::internal::net {

/**
 * @internal
 *
 * @returns `true` if the `address` denotes a valid IPv4 or IPv6 address, or
 * `false` otherwise.
 */
bool is_ip_address_valid(const std::string& address);

/**
 * @internal
 *
 * @returns `true` if the `hostname` denotes a valid hostname, or
 * `false` otherwise.
 */
bool is_hostname_valid(const std::string& hostname);

} // namespace dmitigr::internal::net

#endif  // DMITIGR_INTERNAL_NET_HPP
