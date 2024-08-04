#pragma once
#include "BeatmapMetadata.hpp"
#include <filesystem>
#include <vector>

class Beatmap
{
public:
  Beatmap(const std::filesystem::path& path);

  const std::vector<BeatmapMetadata>& getDifficulties() const
  {
    return this->difficulties;
  }

private:
  std::filesystem::path path;
  std::vector<BeatmapMetadata> difficulties;
};
