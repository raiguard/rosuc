#pragma once
#include "Audio/AudioManager.hpp"
#include "Beatmap/BeatmapManager.hpp"
#include "Graphics/Window.hpp"
#include "NamedBool.hpp"

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

  Window window;
  AudioManager audioManager;
  BeatmapManager beatmapManager;

  static constexpr uint32_t UPS = 1000;
};
