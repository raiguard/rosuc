#include "Beatmap.hpp"
#include "Reader/ZipReader.hpp"
#include "Util.hpp"
#include <cassert>

Beatmap::Beatmap(const std::filesystem::path& path)
  : path(path)
{
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
