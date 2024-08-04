#pragma once
#include "Beatmap/BeatmapMetadata.hpp"

class BeatmapMetadata;

class ActiveBeatmapInfo
{
public:
  ActiveBeatmapInfo(const BeatmapMetadata& metadata) : metadata(metadata) {}

  const BeatmapMetadata& getInfo() const { return this->metadata; }

private:
  const BeatmapMetadata metadata;
};
