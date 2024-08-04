#pragma once
#include "Beatmap/Beatmap.hpp"

class Beatmap;

class ActiveBeatmapInfo
{
public:
  ActiveBeatmapInfo(const Beatmap& metadata);

  const Beatmap& getInfo() const { return this->beatmap; }

private:
  const Beatmap beatmap;
};
