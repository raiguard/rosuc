#pragma once
#include "Audio/AudioManager.hpp"
#include "Beatmap/BeatmapManager.hpp"
#include "Graphics/Window.hpp"
#include "NamedBool.hpp"
#include <memory>

class Game
{
public:
  using ShouldQuit = NamedBool<class ShouldQuitTag>;
  Game();
  ShouldQuit frame();

private:
  ShouldQuit handleEvents();
  ShouldQuit update();
  void drawDebugGui();

  std::unique_ptr<Window> window;
  std::unique_ptr<AudioManager> audioManager;
  std::unique_ptr<BeatmapManager> beatmapManager;

  static constexpr uint32_t UPS = 1000;
};
