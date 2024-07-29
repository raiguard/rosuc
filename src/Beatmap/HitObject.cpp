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

HitObject::HitObject(const std::string& input)
{
  std::istringstream stream(input);
  this->pos = {readNumber<uint16_t>(stream), readNumber<uint16_t>(stream)};
  this->timestamp = readNumber<uint32_t>(stream);
}
