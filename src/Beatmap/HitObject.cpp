#include "Beatmap/HitObject.hpp"
#include "Util.hpp"
#include <cassert>
#include <sstream>

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
}

HitObject::HitObject(const std::string& data)
{
  std::istringstream input(data);
  this->pos = {readNumber<uint16_t>(input, ','), readNumber<uint16_t>(input, ',')};
  this->timestamp = readNumber<uint32_t>(input, ',');
  this->flags = readNumber<uint16_t>(input, ',');
  this->hitSounds = readNumber<uint16_t>(input);

  switch (this->getType())
  {
  case HitObjectType::HitCircle:
    break;
  case HitObjectType::Slider:
    this->readSliderData(input);
    break;
  case HitObjectType::Spinner:
    this->endTime = readNumber<uint32_t>(input);
    break;
  case HitObjectType::ManiaHoldNote:
    break;
  }
}

void HitObject::readSliderData(std::istream& input)
{
  if (input.peek() == ',')
    input.ignore(1);

  {
    char curveType;
    input.read(&curveType, 1);
    switch (curveType)
    {
    case 'B':
      this->curveType = CurveType::Bezier;
      break;
    case 'C':
      this->curveType = CurveType::Cetripedal;
      break;
    case 'L':
      this->curveType = CurveType::Linear;
      break;
    case 'P':
      this->curveType = CurveType::PerfectCircle;
      break;
    default:
      Util::panic("Invalid slider curve type {}", curveType);
    }
  }

  while (input.peek() == '|')
  {
    input.ignore(1);
    auto x = readNumber<uint16_t>(input, ':');
    auto y = readNumber<uint16_t>(input);
    this->curvePoints.emplace_back(x, y);
  }

  assert(input.peek() == ',');
  input.ignore(1);

  this->slides = readNumber<uint16_t>(input, ',');
  this->length = readNumber<float>(input);
}

HitObjectType HitObject::getType() const
{
  uint8_t masked = this->flags & 0b10001011;
  switch (masked)
  {
  case 0b1:
    return HitObjectType::HitCircle;
  case 0b10:
    return HitObjectType::Slider;
  case 0b1000:
    return HitObjectType::Spinner;
  case 0b10000000:
    return HitObjectType::ManiaHoldNote; // TODO: Reject non-standard beatmaps
  default:
    Util::panic("Invalid hit object type {}", masked);
  }
}
