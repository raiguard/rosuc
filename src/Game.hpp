#pragma once
#include "Audio/AudioManager.hpp"
#include "Beatmap/ActiveBeatmapInfo.hpp"
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
  void init();
  ShouldQuit frame();

private:
  ShouldQuit handleEvents();
  void drawDebugGui();

  std::vector<Beatmap> beatmaps;
  std::unique_ptr<Window> window;
  std::unique_ptr<AudioManager> audioManager;
  std::unique_ptr<ActiveBeatmapInfo> activeBeatmap;
  static constexpr uint32_t UPS = 1000;
};
