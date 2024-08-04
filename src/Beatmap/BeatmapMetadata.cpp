#include "Beatmap/BeatmapMetadata.hpp"
#include "Reader/OsuReader.hpp"
#include "Util.hpp"
#include <fstream>
#include <sstream>

std::istream& getLine(std::istream& input, std::string& buf, char delim = '\n')
{
  std::istream& res = std::getline(input, buf, delim);
  Util::trimString(buf);
  return res;
}

std::pair<std::string, std::string> readKeyValue(const std::string& line)
{
  size_t delim = line.find(':');
  if (delim == line.npos)
    Util::panic("Invalid key:value pair");
  std::string key = line.substr(0, delim);
  Util::trimString(key);
  if (line.size() < delim + 1)
    return {key, ""};
  std::string value = line.substr(delim + 1, line.size());
  Util::trimString(value);
  return {key, value};
}

BeatmapMetadata::BeatmapMetadata(const std::filesystem::path& path)
  : path(path)
{
  std::ifstream file(path);
  if (file.fail())
    Util::panic("Unable to read beatmap file {}", path.c_str());

  std::string line;
  getLine(file, line);
  if (line.find("osu file format v14") == line.npos)
    // TODO: Support older formats
    return;

  std::string section;
  while (getLine(file, line))
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
    // else if (section == "HitObjects")
    //   this->hitObjects.emplace_back(line);
    else if (section == "General")
    {
      auto [key, value] = readKeyValue(line);
      if (key == "Mode" && value != "0")
      {
        this->title = "";
        return;
      }
      else if (key == "AudioFilename")
      {
        this->audio = this->path.parent_path() / value;
        // TODO: This is an awful hack to work around the fact that the audio filename is case-insensitive
        bool found = false;
        for (const auto& entry : std::filesystem::directory_iterator(this->path.parent_path()))
          if (Util::iequals(this->audio.filename().c_str(), entry.path().filename().c_str()))
          {
            this->audio = entry.path();
            found = true;
            break;
          }
        if (!found)
          Util::panic("Could not find audio {}", this->audio.c_str());
      }
      else if (key == "PreviewTime")
      {
        std::istringstream stream(value);
        this->previewTime = OsuReaders::readNumber<int32_t>(stream);
      }
    }
  }
}
