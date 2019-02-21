// -*- C++ -*-
// Copyright (C) Dmitry Igrishin
// For conditions of distribution and use, see files LICENSE.txt or internal.hpp

#include "dmitigr/internal/debug.hpp"
#include "dmitigr/internal/os.hpp"

#include <cstdlib>
#include <memory>
#include <system_error>

#ifdef _WIN32

/*
 * For historical reasons, the Windows.h header defaults to including the
 * Winsock.h header file for Windows Sockets 1.1. The declarations in the
 * Winsock.h header file will conflict with the declarations in the Winsock2.h
 * header file required by Windows Sockets 2.0. The WIN32_LEAN_AND_MEAN macro
 * prevents the Winsock.h from being included by the Windows.h header.
 *
 * https://social.msdn.microsoft.com/Forums/vstudio/en-US/671124df-c42b-48b8-a4ac-3413230bc43b/dll-compilationredefinition-error
 */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include <Winnls.h>
#include <Lmcons.h>

// IO headers
#include <io.h>
#include <Stdio.h>

#include <direct.h> // _getcwd()

#else // Unix

#include <pwd.h>
#include <unistd.h>
#include <sys/types.h>

#endif

#include "dmitigr/internal/implementation_header.hpp"

namespace dmitigr::internal::os {

namespace {

inline char* getcwd__(char* buffer, std::size_t size)
{
#ifdef _WIN32
  return _getcwd(buffer, int(size));
#else
  return getcwd(buffer, size);
#endif
}

inline char* dmint_os_cwd()
{
  constexpr std::size_t max_path_size = 128 * 128;
  std::size_t sz = 16;
  auto* buf = static_cast<char*>(std::calloc(1, sz));
  while (!getcwd__(buf, sz)) {
    if (errno == ERANGE) {
      sz *= 2;
      if (sz > max_path_size) {
        std::free(buf);
        return nullptr;
      } else {
        if (auto* new_buf = static_cast<char*>(std::realloc(buf, sz)))
          buf = new_buf;
        else {
          std::free(buf);
          return nullptr;
        }
      }
    } else { // ENOMEM
      DMITIGR_INTERNAL_DOUT_ASSERT(!buf);
      return nullptr;
    }
  }

  return buf;
}

} // namespace

DMITIGR_INTERNAL_INLINE std::string current_working_directory()
{
  std::unique_ptr<char[], void (*)(void*)> guarded{dmint_os_cwd(), &std::free};
  return guarded ? guarded.get() : std::string{};
}

DMITIGR_INTERNAL_INLINE std::string current_username()
{
  std::string result;
#ifdef _WIN32
  constexpr DWORD max_size = UNLEN + 1;
  result.resize(max_size);
  DWORD sz{max_size};
  if (::GetUserName(result.data(), &sz) != 0)
    result.resize(sz - 1);
  else
    throw std::system_error{int(::GetLastError()), std::system_category(), "dmitigr::internal::os::current_username()"};
#else
  struct passwd pwd;
  struct passwd *pwd_ptr{};
  const uid_t uid = geteuid();
  const std::size_t bufsz = []()
  {
    auto result = sysconf(_SC_GETPW_R_SIZE_MAX);
    if (result == -1)
      result = 16384;
    return result;
  }();
  const std::unique_ptr<char[]> buf{new char[bufsz]};
  const int s = getpwuid_r(uid, &pwd, buf.get(), bufsz, &pwd_ptr);
  if (pwd_ptr == nullptr) {
    if (s == 0)
      throw std::logic_error{"current username is unavailable (possible something wrong with the OS)"};
    else
      throw std::system_error{s, std::system_category(), "dmitigr::internal::os::current_username()"};
  } else
    result = pwd.pw_name;
#endif
  return result;
}

DMITIGR_INTERNAL_INLINE std::optional<std::string> environment_variable(const std::string& name)
{
#ifdef _WIN32
  const std::unique_ptr<char, void(*)(void*)> buffer{nullptr, &std::free};
  char* result = buffer.get();
  const auto err = _dupenv_s(&result, nullptr, name.c_str());
  if (err)
    throw std::system_error{err, std::system_category(), "dmitigr::internal::os::environment_variable()"};
#else
  const char* const result = std::getenv(name.c_str());
#endif
  return result ? std::make_optional(std::string{result}) : std::nullopt;
}

// -----------------------------------------------------------------------------

namespace io {

DMITIGR_INTERNAL_INLINE std::size_t seek(const int fd, long const offset, const Origin whence)
{
#ifdef _WIN32
  const auto result = ::_lseek(fd, offset, int(whence));
#else
  const auto result = ::lseek(fd, offset, int(whence));
#endif

  if (result < 0) {
    const int err = errno;
    throw std::system_error{err, std::system_category(), "dmitigr::http::internal::io::seek()"};
  } else
    return std::size_t(result);
}

DMITIGR_INTERNAL_INLINE std::size_t read(const int fd, void* const buffer, const unsigned int count)
{
  DMITIGR_INTERNAL_ASSERT(buffer);

#ifdef _WIN32
  const auto result = ::_read(fd, buffer, count);
#else
  const auto result = ::read(fd, buffer, count);
#endif

  if (result < 0) {
    const int err = errno;
    throw std::system_error{err, std::system_category(), "dmitigr::http::internal::io::read()"};
  } else
    return std::size_t(result);
}

} // namespace io

} // namespace dmitigr::internal::os

#include "dmitigr/internal/implementation_footer.hpp"
