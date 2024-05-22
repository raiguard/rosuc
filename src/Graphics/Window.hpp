#pragma once

typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;

class Window
{
public:
  Window();
  ~Window();

  void draw();
private:
  SDL_Window* sdlWindow;
  SDL_Renderer* sdlRenderer;
};
