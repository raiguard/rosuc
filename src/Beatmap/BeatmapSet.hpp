#pragma once
#include "Beatmap.hpp"
#include <filesystem>
#include <vector>

class BeatmapSet
{
public:
  BeatmapSet(const std::filesystem::path& path);

  const std::vector<Beatmap>& getDifficulties() const { return this->difficulties; }

private:
  std::filesystem::path path;
  std::vector<Beatmap> difficulties;
};
