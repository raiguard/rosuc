#pragma once
#include "Beatmap/Beatmap.hpp"

class Beatmap;

class ActiveBeatmapInfo
{
public:
  ActiveBeatmapInfo(const Beatmap& metadata);

  const Beatmap& getInfo() const { return this->metadata; }

private:
  const Beatmap metadata;
};
