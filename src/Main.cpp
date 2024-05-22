#include "Beatmap/Beatmap.hpp"
#include <fmt/format.h>
#include <SDL3/SDL.h>

const char* PATH = "beatmaps/404649 yuikonnu - Yume Chizu [no video].zip";

int main()
{
  // SDL_SetHint(SDL_HINT_EVENT_LOGGING, "2");
  if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0)
    throw std::runtime_error(fmt::format("Failed to initialize SDL: {}", SDL_GetError()));

  SDL_Window* window = SDL_CreateWindow("Rai's osu!standard clone", 800, 600, SDL_WINDOW_RESIZABLE);
  if (!window)
    throw std::runtime_error(fmt::format("Failed to create SDL window: {}", SDL_GetError()));

  SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);
  if (!renderer)
    throw std::runtime_error(fmt::format("Failed to create SDL renderer: {}", SDL_GetError()));

  Beatmap beatmap(PATH);

  bool quit = false;
  while (!quit)
  {
    SDL_Event event;
    while (SDL_PollEvent(&event))
      if (event.type == SDL_EVENT_QUIT)
        quit = true;

    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
