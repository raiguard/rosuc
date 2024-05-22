#include "Beatmap.hpp"
#include "Util.hpp"
#include "ZipReader.hpp"
#include <cassert>
#include <fmt/core.h>

Beatmap::Beatmap(const std::filesystem::path& path)
  : path(path)
{
  ZipReader reader(path);
  reader.setPattern("*.osu");
  if (!reader.gotoFirstEntry())
    Util::panic("Failed to read entry from beatmap file");

  do
    this->difficulties.emplace_back(reader.readEntry());
  while (reader.gotoNextEntry());
}
