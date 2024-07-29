#pragma once
#include "Beatmap/OsuPixel.hpp"
#include <cassert>
#include <cstdint>
#include <string>
#include <vector>

class HitSounds
{
public:
  HitSounds() = default;
  HitSounds(uint8_t input) : value(input) {};

  bool isNormal() const { return this->value & 0; }
  bool isWhistle() const { return this->value & 1; }
  bool isFinish() const { return this->value & (1 << 1); }
  bool isClap() const { return this->value & (1 << 2); }
private:
  uint8_t value = 0;
};

enum class HitObjectType
{
  HitCircle,
  Slider,
  Spinner,
  ManiaHoldNote,
};

enum class CurveType
{
  Bezier,
  Cetripedal,
  Linear,
  PerfectCircle,
};

class HitObject
{
public:
  HitObject(const std::string& input);

  OsuPixel getPos() const { return this->pos; }
  uint32_t getTimestamp() const { return this->timestamp; }

  HitObjectType getType() const;
  bool isNewCombo() const { return this->flags & (1 << 2); }
  uint8_t getComboSkip() const { return (this->flags & (7 << 4)) >> 4; }

  HitSounds getHitSounds() const { return this->hitSounds; }

  CurveType getCurveType() const { assert(this->getType() == HitObjectType::Slider); return this->curveType; }
  const std::vector<OsuPixel>& getCurvePoints() const { assert(this->getType() == HitObjectType::Slider); return this->curvePoints; }
private:
  void readSliderData(std::istream& input);

  // Common
  OsuPixel pos;
  uint32_t timestamp = 0;
  uint8_t flags = 0;
  HitSounds hitSounds;

  // Slider
  CurveType curveType;
  std::vector<OsuPixel> curvePoints;
  uint8_t slides = 0;
  float length = 0;

  // Spinner
  uint32_t endTime = 0;
};
