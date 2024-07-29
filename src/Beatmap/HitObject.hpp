#pragma once
#include "Beatmap/OsuPixel.hpp"
#include <cstdint>
#include <string>

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
  uint8_t value;
};

enum class HitObjectType
{
  HitCircle,
  Slider,
  Spinner,
  ManiaHoldNote, // TODO: Figure out how to reject beatmap if it's not osu!standard
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
private:
  OsuPixel pos;
  uint32_t timestamp;
  uint8_t flags;
  HitSounds hitSounds;
};
