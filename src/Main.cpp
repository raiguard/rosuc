#include "Beatmap/Beatmap.hpp"
#include "Graphics/Window.hpp"

#include <fmt/format.h>
#include <SDL3/SDL.h>
#include <thread>

typedef bool ShouldQuit;

ShouldQuit handleEvents(Window& window)
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_EVENT_QUIT)
      return true;
  }

  return false;
}

ShouldQuit mainLoopStep(Window& window)
{
  using Clock = std::chrono::high_resolution_clock;
  using namespace std::chrono_literals;

  static Clock::time_point lastFrameTime = Clock::now();
  static Clock::duration accumulator;
  static constexpr std::chrono::nanoseconds timestep(int(1000000000 / 1000));

  Clock::time_point currentTime = Clock::now();
  Clock::duration frameTime = currentTime - lastFrameTime;
  lastFrameTime = currentTime;

  accumulator += frameTime;

  if (handleEvents(window))
    return true;

  while (accumulator >= timestep)
  {
    // state.update();
    accumulator -= timestep;
  }

  window.draw();

  // Cap framerate at max simulation speed
  std::chrono::duration thisFrameTime = Clock::now() - lastFrameTime;
  Clock::duration timeToSleep = timestep - accumulator - thisFrameTime;
  if (timeToSleep > Clock::duration::zero())
    std::this_thread::sleep_for(timeToSleep);

  return false;
}

int main()
{
  Beatmap beatmap("beatmaps/404649 yuikonnu - Yume Chizu [no video].zip");

  Window window;
  while (!mainLoopStep(window));

  return 0;
}
