#include "Game.hpp"
#include "Graphics/Window.hpp"
#include "Util.hpp"
#include <SDL3/SDL_render.h>
#include <imgui_impl_sdl3.h>
#include <SDL3/SDL_events.h>
#include <thread>

void Game::initBeatmaps(const std::filesystem::path& path)
{
  if (!std::filesystem::exists(path))
    Util::panic("Unknown path '{}'", path.c_str());

  if (!std::filesystem::is_directory(path))
    Util::panic("'{}' is not a directory.", path.c_str());

  for (const auto& entry : std::filesystem::directory_iterator(path))
  {
    if (entry.is_regular_file() && entry.path().extension() == ".osz")
      this->beatmaps.emplace_back(entry.path());
  }
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

  this->window->beginDrawing();
  this->drawDebugGui();
  this->window->finishDrawing();

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
    if (ImGui_ImplSDL3_ProcessEvent(&event))
      continue;
    if (event.type == SDL_EVENT_QUIT)
      return true;
  }

  return false;
}

void Game::drawDebugGui()
{
  ImGuiIO& io = ImGui::GetIO();
  ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
  ImGui::Text("Video driver: %s", SDL_GetCurrentVideoDriver());
  ImGui::Text("Render: %.1f FPS (%.3f ms/frame)", io.Framerate, 1000.0f / io.Framerate);
  // if (ImGui::Checkbox("Use vsync", &this->useVsync))
  //   SDL_SetRenderVSync(this->useVsync ? 1 : 0);
  auto trueSize = window->getTrueSize();
  ImGui::Text("True resolution: %dx%d", trueSize.first, trueSize.second);
  auto scaledSize = window->getScaledSize();
  ImGui::Text("Scaled resolution: %dx%d", scaledSize.first, scaledSize.second);
  ImGui::Text("Display density: %fx", double(trueSize.first) / scaledSize.first);
  ImGui::End();

  ImGui::Begin("Beatmaps");
  for (const Beatmap& beatmap : this->beatmaps)
    for (const BeatmapInfo& difficulty : beatmap.getDifficulties())
      ImGui::Text("%s (%s)", difficulty.title.c_str(), difficulty.version.c_str());
  ImGui::End();
}
