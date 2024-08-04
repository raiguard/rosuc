#pragma once
#include "Beatmap/ActiveBeatmapInfo.hpp"
#include "Beatmap/BeatmapSet.hpp"
#include <vector>

class BeatmapManager
{
public:
  BeatmapManager() = default;
  void loadDirectory(const std::filesystem::path& path);

  void setActive(const Beatmap& beatmap) { this->activeBeatmap = std::make_unique<ActiveBeatmapInfo>(beatmap); }

  const std::vector<BeatmapSet>& getSets() const { return this->sets; }
  const std::unique_ptr<ActiveBeatmapInfo>& getActiveBeatmap() const { return this->activeBeatmap; }

private:
  std::vector<BeatmapSet> sets;
  std::unique_ptr<ActiveBeatmapInfo> activeBeatmap;
};
