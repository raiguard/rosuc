#include "BeatmapInfo.hpp"
#include "Util.hpp"
#include <sstream>

void trimString(std::string& s)
{
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char ch) { return !std::isspace(ch); }));
  s.erase(std::find_if(s.rbegin(), s.rend(), [](char ch) { return !std::isspace(ch); }).base(), s.end());
}

std::pair<std::string, std::string> readKeyValue(const std::string& line)
{
  size_t delim = line.find(':');
  std::string key = line.substr(0, delim);
  trimString(key);
  if (line.size() < delim + 1)
    return {key, ""};
  std::string value = line.substr(delim + 1, line.size());
  trimString(value);
  return {key, value};
}

BeatmapInfo::BeatmapInfo(const std::string& data)
{
  std::istringstream input(data);
  std::string line;
  std::getline(input, line);
  trimString(line);
  if (line.find("osu file format v14") == line.npos)
    // TODO: Support older formats
    return;

  bool foundMeta = false;
  while (std::getline(input, line))
  {
    trimString(line);
    if (line == "[Metadata]")
    {
      foundMeta = true;
      break;
    }
  }
  while (std::getline(input, line))
  {
    trimString(line);
    if (line.empty())
      continue;
    if (line[0] == '[')
      break;
    auto [key, value] = readKeyValue(line);
    if (key == "Title")
      this->title = value;
    else if (key == "TitleUnicode")
      this->titleUnicode = value;
    else if (key == "Artist")
      this->artist = value;
    else if (key == "ArtistUnicode")
      this->artistUnicode = value;
    else if (key == "Creator")
      this->creator = value;
    else if (key == "Version")
      this->version = value;
    else if (key == "Source")
      this->source = value;
    else if (key == "Tags")
      this->tags = value;
    else if (key == "BeatmapID")
      this->beatmapID = value;
    else if (key == "BeatmapSetID")
      this->beatmapSetID = value;
    else
      Util::panic("Invalid key in beatmap metadata: {}", key);
  }
  if (!foundMeta)
    Util::panic("Invalid beatmap file - could not find metadata");
}
