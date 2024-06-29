#pragma once
#include <SDL3/SDL_video.h>
#include <utility>

class Window
{
public:
  Window();
  ~Window();

  void beginDrawing();
  void finishDrawing();

  void setVsync(bool value);

  void drawDebugGui();

  float getDisplayScale();
  float getPixelDensity();

  std::pair<int, int> getTrueSize();
  std::pair<int, int> getScaledSize();

private:
  SDL_Window* sdlWindow;
  SDL_GLContext glContext;
};
