#pragma once
#include "Beatmap/HitObject.hpp"
#include <string>
#include <vector>

class BeatmapInfo
{
public:
  BeatmapInfo(const std::string& data);

  bool isValid()
  {
    return !title.empty();
  }

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

  std::vector<HitObject> hitObjects;
};
