#pragma once
#include <string>

class BeatmapInfo
{
public:
  BeatmapInfo(const std::string& data);

private:
  std::string title;
  std::string titleUnicode;
  std::string artist;
  std::string artistUnicode;
  std::string creator;
  std::string version;
  std::string source;
  std::string tags;
  std::string beatmapID;
  std::string beatmapSetID;
};
