#pragma once

#include <utility>
typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;

class Window
{
public:
  Window();
  ~Window();

  void beginDrawing();
  void finishDrawing();

  void setVsync(bool value);

  void drawDebugGui();

  std::pair<int, int> getTrueSize();
  std::pair<int, int> getScaledSize();

private:
  SDL_Window* sdlWindow;
  SDL_Renderer* sdlRenderer;
};
