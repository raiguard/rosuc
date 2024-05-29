#pragma once

#include <filesystem>
#include <fmt/core.h>

namespace Util
{
  template <typename... T>
  inline void panic(fmt::format_string<T...> format, T&&... args)
  {
    throw std::runtime_error(fmt::format(format, std::forward<T>(args)...));
  }

  std::string readFile(const std::filesystem::path& path);
}
