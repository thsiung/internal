// -*- C++ -*-
// Copyright (C) Dmitry Igrishin
// For conditions of distribution and use, see files LICENSE.txt or internal.hpp

#ifndef DMITIGR_INTERNAL_DEBUG_HPP
#define DMITIGR_INTERNAL_DEBUG_HPP

#include "dmitigr/internal/macros.hpp"

#include <cstdio>
#include <stdexcept>
#include <string>

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

#define DMITIGR_INTERNAL_REQUIRE__(req, msg) {                          \
    if (!(req)) {                                                       \
      std::string message{"API requirement (" #msg ") violated"};       \
      if constexpr (dmitigr::internal::is_debug_enabled) {              \
        message.append(" at " __FILE__ ":").append(std::to_string(int(__LINE__))); \
      }                                                                 \
      throw std::logic_error{message};                                  \
    }                                                                   \
  }

#define DMITIGR_INTERNAL_REQUIRE(req)       DMITIGR_INTERNAL_REQUIRE__(req, req)
#define DMITIGR_INTERNAL_REQUIRE2(req, msg) DMITIGR_INTERNAL_REQUIRE__(req, msg)

namespace dmitigr::internal::debug {

class Requirements_violation : public std::logic_error {
public:
  explicit Requirements_violation(std::string context)
    : logic_error{std::string{"API requirement violated in the following context: "}.append(std::move(context))}
  {}

  Requirements_violation(std::string requirement, std::string context)
    : logic_error{std::string{"API requirement ("}.append(std::move(requirement))
                  .append(") violated in the following context: ").append(std::move(context))}
  {}
};

template<typename T, typename ... Types>
inline void require(const T& req, Types&& ... args)
{
  if (!req)
    throw Requirements_violation{std::forward<Types>(args)...};
}

} // namespace dmitigr::internal::debug

#endif  // DMITIGR_INTERNAL_DEBUG_HPP
