#include "Beatmap.hpp"
#include "Reader/ZipReader.hpp"
#include "Util.hpp"
#include <cassert>
#include <print>

Beatmap::Beatmap(const std::filesystem::path& path)
  : path(path)
{
  std::println("Reading beatmap {}", path.c_str());
  ZipReader reader(path);
  reader.setPattern("*.osu");
  if (!reader.gotoFirstEntry())
    Util::panic("Beatmap file contains no difficulties.");

  do {
    BeatmapInfo info(reader.readEntry());
    if (info.isValid())
      this->difficulties.emplace_back(std::move(info));
  } while (reader.gotoNextEntry());
}
