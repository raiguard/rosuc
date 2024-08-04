#include "Beatmap.hpp"
#include <cassert>
#include <print>

Beatmap::Beatmap(const std::filesystem::path& path)
  : path(path)
{
  std::println("Reading beatmap {}", path.c_str());
  for (const auto& entry : std::filesystem::directory_iterator(path))
    if (entry.is_regular_file() && entry.path().extension() == ".osu")
    {
      BeatmapMetadata meta(entry.path());
      if (meta.isValid())
        this->difficulties.emplace_back(std::move(meta));
    }
}
