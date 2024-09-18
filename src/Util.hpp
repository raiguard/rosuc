#pragma once
#include <filesystem>
#include <iostream>
#include <SDL_messagebox.h>

namespace Util
{
  template <typename... T>
  [[noreturn]]
  inline void panic(std::format_string<T...> format, T&&... args)
  {
    std::string err = std::format(format, std::forward<T>(args)...);
    std::println(std::cerr, "ERROR: {}", err);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Critical error", err.c_str(), nullptr);
    std::exit(1);
  }

  std::string readFile(const std::filesystem::path& path);
  void trimString(std::string& s);
  bool iequals(std::string_view lhs, std::string_view rhs);
  std::string toLower(std::string_view str);
  bool icontains(std::string_view str, std::string_view search);
}
