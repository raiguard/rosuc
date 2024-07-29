#include "Beatmap/HitObject.hpp"
#include <cassert>
#include <sstream>

template<typename T>
T readNumber(std::istringstream& stream)
{
  T val;
  stream >> val;
  stream.ignore(1); // Delimiter
  return val;
}

HitObject::HitObject(const std::string& input)
{
  std::istringstream stream(input);
  this->pos = {readNumber<uint16_t>(stream), readNumber<uint16_t>(stream)};
  this->timestamp = readNumber<uint32_t>(stream);
  this->flags = readNumber<uint8_t>(stream);
  this->hitSounds = readNumber<uint8_t>(stream);
}
