#pragma once
#include "BeatmapInfo.hpp"
#include <filesystem>
#include <vector>

class Beatmap
{
public:
  Beatmap(const std::filesystem::path& path);

  const std::vector<BeatmapInfo> getDifficulties() const
  {
    return this->difficulties;
  }

private:
  std::filesystem::path path;
  std::vector<BeatmapInfo> difficulties;
};
