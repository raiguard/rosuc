#include "Window.hpp"
#include "Util.hpp"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>

Window::Window()
{
  // SDL_SetHint(SDL_HINT_EVENT_LOGGING, "2");
  if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0)
    Util::panic("Failed to initialize SDL: {}", SDL_GetError());

  this->sdlWindow = SDL_CreateWindow("Rai's osu!standard clone", 800, 600, SDL_WINDOW_RESIZABLE);
  if (!this->sdlWindow)
    Util::panic("Failed to create SDL window: {}", SDL_GetError());

  this->sdlRenderer = SDL_CreateRenderer(this->sdlWindow, nullptr);
  if (!this->sdlRenderer)
    Util::panic("Failed to create SDL renderer: {}", SDL_GetError());
}

Window::~Window()
{
  SDL_DestroyRenderer(this->sdlRenderer);
  SDL_DestroyWindow(this->sdlWindow);
  SDL_Quit();
}

void Window::draw()
{
  SDL_RenderClear(this->sdlRenderer);
  SDL_RenderPresent(this->sdlRenderer);
}
