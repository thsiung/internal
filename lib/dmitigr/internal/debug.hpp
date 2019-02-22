// -*- C++ -*-
// Copyright (C) Dmitry Igrishin
// For conditions of distribution and use, see files LICENSE.txt or internal.hpp

#ifndef DMITIGR_INTERNAL_DEBUG_HPP
#define DMITIGR_INTERNAL_DEBUG_HPP

#include "dmitigr/internal/macros.hpp"

#include <cstdio>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace dmitigr::internal {

#ifdef NDEBUG
constexpr bool is_debug_enabled = false;
#else
constexpr bool is_debug_enabled = true;
#endif

} // namespace dmitigr::internal

#define DMITIGR_INTERNAL_DOUT__(...) {                      \
    std::fprintf(stderr, "Debug output from " __FILE__ ":"  \
      DMITIGR_INTERNAL_XSTR__(__LINE__) ": " __VA_ARGS__);  \
  }

#define DMITIGR_INTERNAL_ASSERT__(a, t) {                               \
    if (!(a)) {                                                         \
      DMITIGR_INTERNAL_DOUT__("assertion (%s) failed\n", #a)            \
        if constexpr (t) {                                              \
          throw std::logic_error{std::string{"assertion (" #a ") failed at " __FILE__ ":"} \
            .append(std::to_string(int(__LINE__)))};                    \
        }                                                               \
    }                                                                   \
  }

#define DMITIGR_INTERNAL_DOUT_ALWAYS(...)      DMITIGR_INTERNAL_DOUT__(__VA_ARGS__)
#define DMITIGR_INTERNAL_DOUT_ASSERT_ALWAYS(a) DMITIGR_INTERNAL_ASSERT__(a, false)
#define DMITIGR_INTERNAL_ASSERT_ALWAYS(a)      DMITIGR_INTERNAL_ASSERT__(a, true)

#define DMITIGR_INTERNAL_IF_DEBUG__(code) if constexpr (dmitigr::internal::is_debug_enabled) { code }

#define DMITIGR_INTERNAL_DOUT(...)      { DMITIGR_INTERNAL_IF_DEBUG__(DMITIGR_INTERNAL_DOUT_ALWAYS(__VA_ARGS__)) }
#define DMITIGR_INTERNAL_DOUT_ASSERT(a) { DMITIGR_INTERNAL_IF_DEBUG__(DMITIGR_INTERNAL_DOUT_ASSERT_ALWAYS(a)) }
#define DMITIGR_INTERNAL_ASSERT(a)      { DMITIGR_INTERNAL_IF_DEBUG__(DMITIGR_INTERNAL_ASSERT_ALWAYS(a)) }

// -----------------------------------------------------------------------------

// TODO: DMITIGR_INTERNAL_REQUIRE* macros are deprecated. Remove them after switching to require<> everywhere.
#define DMITIGR_INTERNAL_REQUIRE__(req, msg) {                          \
    if (!(req)) {                                                       \
      std::string message{"API requirement (" #msg ") violated"};       \
      message.append(" at " __FILE__ ":").append(std::to_string(int(__LINE__))); \
      throw std::logic_error{message};                                  \
    }                                                                   \
  }

#define DMITIGR_INTERNAL_REQUIRE(req)       DMITIGR_INTERNAL_REQUIRE__(req, req)
#define DMITIGR_INTERNAL_REQUIRE2(req, msg) DMITIGR_INTERNAL_REQUIRE__(req, msg)

namespace dmitigr::internal::debug {

/**
 * @brief Represents a convenient wrapper for `std::runtime_error` class.
 */
class Runtime_error : public std::runtime_error {
public:
  explicit Runtime_error(std::string ns, std::string_view cl_or_fn, std::string_view message)
    : runtime_error{ns.append("::").append(cl_or_fn).append(": ").append(message)}
  {}
};

// -----------------------------------------------------------------------------

namespace detail {

/**
 * @returns The string with the exception message.
 */
inline std::string req_vio_msg__(std::string context, const char* const details)
{
  context.append(": API requirement");
  if (details)
    context.append(" (").append(details).append(")");
  context.append(" violated");
  return context;
}

} // namespace detail

/**
 * @brief Checks the requirement `req` and throws the object of type `E` if the
 * requirement is violated (i.e. if `!req == true`).
 */
template<class E, typename T>
inline void require(const T& req, const char* const ns, const char* const fn, const char* const details)
{
  static_assert(std::is_base_of_v<std::logic_error, E>);
  DMITIGR_INTERNAL_ASSERT(ns && fn);
  if (!req)
    throw E{detail::req_vio_msg__(std::string{ns}.append("::").append(fn), details)};
}

/**
 * @overload
 */
template<class E, typename T>
inline void require(const T& req, const char* const ns, const char* const cl, const char* const fn, const char* const details)
{
  static_assert(std::is_base_of_v<std::logic_error, E>);
  DMITIGR_INTERNAL_ASSERT(ns && cl && fn);
  if (!req)
    throw E{detail::req_vio_msg__(std::string{ns}.append("::").append(cl).append("::").append(fn), details)};
}

// -----------------------------------------------------------------------------

} // namespace dmitigr::internal::debug

#endif  // DMITIGR_INTERNAL_DEBUG_HPP
