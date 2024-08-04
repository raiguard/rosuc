#pragma once

#include <filesystem>

namespace Util
{
  template <typename... T>
  [[noreturn]]
  inline void panic(std::format_string<T...> format, T&&... args)
  {
    throw std::runtime_error(std::format(format, std::forward<T>(args)...));
  }

  std::string readFile(const std::filesystem::path& path);
  void trimString(std::string& s);
  bool iequals(std::string_view lhs, std::string_view rhs);
  std::string toLower(std::string_view str);
  bool icontains(std::string_view str, std::string_view search);
}
