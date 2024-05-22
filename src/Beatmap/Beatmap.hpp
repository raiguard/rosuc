#pragma once
#include "BeatmapInfo.hpp"
#include <filesystem>
#include <vector>

class Beatmap
{
public:
  Beatmap(const std::filesystem::path& path);

private:
  std::filesystem::path path;
  std::vector<BeatmapInfo> difficulties;
};
