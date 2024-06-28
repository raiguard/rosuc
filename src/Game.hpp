#pragma once

#include "Beatmap/Beatmap.hpp"
#include "Graphics/Window.hpp"
#include <filesystem>
#include <memory>
#include <vector>

class Game
{
public:
  using ShouldQuit = bool;

  void initBeatmaps(const std::filesystem::path& path);
  void initWindow();
  ShouldQuit frame();


private:
  ShouldQuit handleEvents();
  void drawDebugGui();

  std::vector<Beatmap> beatmaps;
  std::unique_ptr<Window> window;

  static constexpr uint32_t UPS = 1000;

  std::optional<BeatmapInfo> activeBeatmap;
};
