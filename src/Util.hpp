#pragma once

#include <fmt/core.h>

namespace Util
{
  template <typename... T>
  inline void panic(fmt::format_string<T...> format, T&&... args)
  {
    throw std::runtime_error(fmt::format(format, std::forward<T>(args)...));
  }
}
