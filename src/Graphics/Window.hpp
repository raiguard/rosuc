#pragma once
#include <SDL_video.h>
#include <utility>

class Window
{
public:
  Window();
  ~Window();

  void beginDrawing();
  void extracted();
  void finishDrawing();

  void setVsync(bool value);

  void drawDebugGui();

  std::pair<int, int> getTrueSize();
  std::pair<int, int> getScaledSize();

  void testGraphics();

private:
  SDL_Window* sdlWindow;
  SDL_GLContext glContext;

  uint32_t triangleShader;
  uint32_t triangleVBO;
};
