// -*- C++ -*-
// Copyright (C) Dmitry Igrishin
// For conditions of distribution and use, see files LICENSE.txt or internal.hpp

#include "dmitigr/internal/debug.hpp"
#include "dmitigr/internal/filesystem.hpp"
#include "dmitigr/internal/stream.hpp"

#include <stdexcept>

namespace fs = dmitigr::internal::filesystem;

std::vector<std::filesystem::path> fs::files_by_extension(const std::filesystem::path& root,
  const std::filesystem::path& extension, const bool recursive, const bool include_heading)
{
  std::vector<std::filesystem::path> result;

  if (is_regular_file(root) && root.extension() == extension)
    return {root};

  if (include_heading) {
    auto heading_file = root;
    heading_file.replace_extension(extension);
    if (is_regular_file(heading_file))
      result.push_back(heading_file);
  }

  if (is_directory(root)) {
    const auto traverse = [&](auto iterator)
    {
      for (const auto& dirent : iterator) {
        const auto& path = dirent.path();
        if (is_regular_file(path) && path.extension() == extension)
          result.push_back(dirent);
      }
    };

    if (recursive)
      traverse(std::filesystem::recursive_directory_iterator{root});
    else
      traverse(std::filesystem::directory_iterator{root});
  }
  return result;
}

std::string fs::read_to_string(const std::filesystem::path& path)
{
  std::ifstream stream(path, std::ios_base::in | std::ios_base::binary);
  if (stream)
    return stream::read_to_string(stream);
  else
    throw std::runtime_error{"unable to open file \"" + path.generic_string() + "\""};
}

std::filesystem::path fs::relative_root_path(const std::filesystem::path& indicator)
{
  auto path = std::filesystem::current_path();
  while (true) {
    if (is_directory(path / indicator))
      return path;
    else if (path.has_relative_path())
      path = path.parent_path();
    else
      throw std::runtime_error{"no " + indicator.string() + " directory found"};
  }
}
