#include "Beatmap/HitObject.hpp"
#include <cassert>
#include <sstream>

template<typename T>
T readNumber(std::istringstream& stream)
{
  T val;
  stream >> val;
  assert(stream.peek() == ',');
  stream.ignore(1);
  return val;
}

HitObject::HitObject(std::istringstream& input)
  : pos({readNumber<uint16_t>(input), readNumber<uint16_t>(input)})
  , timestamp(readNumber<uint32_t>(input))
{
  // TODO: Actually read all of the info
  std::string garbage;
  std::getline(input, garbage);
}
