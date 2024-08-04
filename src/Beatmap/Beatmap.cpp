#include "Beatmap.hpp"
#include "Util.hpp"
#include <cassert>
#include <print>

Beatmap::Beatmap(const std::filesystem::path& path)
  : path(path)
{
  std::println("Reading beatmap {}", path.c_str());
  for (const auto& entry : std::filesystem::directory_iterator(path))
    if (entry.is_regular_file() && entry.path().extension() == ".osu")
    {
      BeatmapMetadata meta(Util::readFile(entry.path()));
      if (meta.isValid())
        this->difficulties.emplace_back(std::move(meta));
    }

  // ZipReader reader(path);
  // reader.setPattern("*.osu");
  // if (!reader.gotoFirstEntry())
  //   Util::panic("Beatmap file contains no difficulties.");

  // do {
  // } while (reader.gotoNextEntry());
}
