#pragma once
#include "Audio/AudioManager.hpp"
#include "Beatmap/ActiveBeatmapInfo.hpp"
#include "Beatmap/BeatmapSet.hpp"
#include "Graphics/Window.hpp"
#include "NamedBool.hpp"
#include <filesystem>
#include <memory>
#include <vector>

class Game
{
public:
  using ShouldQuit = NamedBool<class ShouldQuitTag>;

  void initBeatmaps(const std::filesystem::path& path);
  void init();
  ShouldQuit frame();

private:
  ShouldQuit handleEvents();
  ShouldQuit update();
  void drawDebugGui();

  std::vector<BeatmapSet> beatmaps;
  std::unique_ptr<Window> window;
  std::unique_ptr<AudioManager> audioManager;
  std::unique_ptr<ActiveBeatmapInfo> activeBeatmap;
  static constexpr uint32_t UPS = 1000;
};
