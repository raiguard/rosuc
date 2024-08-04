#pragma once
#include <cassert>
#include <istream>

namespace OsuReaders
{
  template <typename T>
  T readNumber(std::istream& input, char delimiter = 0)
  {
    T val;
    input >> val;
    if (delimiter)
    {
      assert(input.peek() == delimiter);
      input.ignore(1);
    }
    return val;
  };
}
