#pragma once

#include <filesystem>

namespace Util
{
  template <typename... T>
  inline void panic(std::format_string<T...> format, T&&... args)
  {
    throw std::runtime_error(std::format(format, std::forward<T>(args)...));
  }

  std::string readFile(const std::filesystem::path& path);
}
