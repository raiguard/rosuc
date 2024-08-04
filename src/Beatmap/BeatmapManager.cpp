#include "Beatmap/BeatmapManager.hpp"
#include "Util.hpp"
#include <print>

void BeatmapManager::loadDirectory(const std::filesystem::path& path)
{
  if (!std::filesystem::exists(path))
  {
    std::println("Cannot load beatmaps at '{}': directory does not exist.", path.c_str());
    return;
  }

  if (!std::filesystem::is_directory(path))
    Util::panic("'{}' is not a directory.", path.c_str());

  uint32_t i = 0;
  for (const auto& entry : std::filesystem::directory_iterator(path))
    if (entry.is_directory())
    {
      BeatmapSet set(entry.path());
      if (set.getBeatmaps().empty())
        continue;
      i += set.getBeatmaps().size();
      this->sets.emplace_back(std::move(set));
    }

  std::sort(this->sets.begin(), this->sets.end(), [](const BeatmapSet& a, const BeatmapSet& b)
  {
    return a.getBeatmaps()[0].title < b.getBeatmaps()[0].title;
  });

  std::println("Loaded {} beatmaps", i);
}
