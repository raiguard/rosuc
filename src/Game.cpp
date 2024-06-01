#include "Game.hpp"
#include "Beatmap/Beatmap.hpp"
#include "Graphics/Window.hpp"
#include <SDL3/SDL_events.h>
#include <thread>

Beatmap& Game::readBeatmap(std::filesystem::path path)
{
  return this->beatmaps.emplace_back(std::move(path));
}

void Game::initWindow()
{
  this->window.reset(new Window());
}

Game::ShouldQuit Game::frame()
{
  using Clock = std::chrono::high_resolution_clock;
  using namespace std::chrono_literals;

  static Clock::time_point lastFrameTime = Clock::now();
  static Clock::duration accumulator;
  static constexpr std::chrono::nanoseconds timestep(1000000000 / 1000);

  Clock::time_point currentTime = Clock::now();
  Clock::duration frameTime = currentTime - lastFrameTime;
  lastFrameTime = currentTime;

  accumulator += frameTime;

  // TODO: Handle input or rendering on a separate thread to make input precision FPS-agnostic
  if (this->handleEvents())
    return true;

  while (accumulator >= timestep)
  {
    // TODO: Update game state
    accumulator -= timestep;
  }

  this->window->draw();

  // Cap framerate at max simulation speed
  std::chrono::duration thisFrameTime = Clock::now() - lastFrameTime;
  Clock::duration timeToSleep = timestep - accumulator - thisFrameTime;
  if (timeToSleep > Clock::duration::zero())
    std::this_thread::sleep_for(timeToSleep);

  return false;
}

Game::ShouldQuit Game::handleEvents()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (event.type == SDL_EVENT_QUIT)
      return true;
  }

  return false;
}
