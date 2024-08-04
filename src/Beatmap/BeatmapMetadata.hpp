#pragma once
#include <filesystem>
#include <string>

class BeatmapMetadata
{
public:
  BeatmapMetadata(const std::filesystem::path& path);

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

  std::filesystem::path path;
  std::filesystem::path audio;
  int32_t previewTime = -1;
};
