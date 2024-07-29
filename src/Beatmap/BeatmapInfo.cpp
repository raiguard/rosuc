#include "BeatmapInfo.hpp"
#include "Util.hpp"
#include <sstream>

void trimString(std::string& s)
{
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char ch) { return !std::isspace(ch); }));
  s.erase(std::find_if(s.rbegin(), s.rend(), [](char ch) { return !std::isspace(ch); }).base(), s.end());
}

std::istream& getLine(std::istream& input, std::string& buf, char delim = '\n')
{
  std::istream& res = std::getline(input, buf, delim);
  trimString(buf);
  return res;
}

std::pair<std::string, std::string> readKeyValue(const std::string& line)
{
  size_t delim = line.find(':');
  if (delim == line.npos)
    Util::panic("Invalid key:value pair");
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
  getLine(input, line);
  if (line.find("osu file format v14") == line.npos)
    // TODO: Support older formats
    return;

  std::string section;
  while (getLine(input, line))
  {
    if (line.empty() || line.rfind("//", 0) != line.npos)
      continue;
    if (line[0] == '[' && line[line.size() - 1] == ']')
    {
      section = line.substr(1, line.size() - 2);
      continue;
    }

    if (section == "Metadata")
    {
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
    else if (section == "HitObjects")
      this->hitObjects.emplace_back(line);
  }
}
