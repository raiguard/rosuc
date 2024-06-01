#include "Beatmap.hpp"
#include "Reader/ZipReader.hpp"
#include "Util.hpp"
#include <cassert>
#include <fmt/core.h>

Beatmap::Beatmap(const std::filesystem::path& path)
  : path(path)
{
  // fmt::println("Reading beatmap {}", path.c_str());
  ZipReader reader(path);
  reader.setPattern("*.osu");
  if (!reader.gotoFirstEntry())
    Util::panic("Failed to read entry from beatmap file");

  do {
    BeatmapInfo info(reader.readEntry());
    if (info.isValid())
      this->difficulties.emplace_back(std::move(info));
  } while (reader.gotoNextEntry());
}
