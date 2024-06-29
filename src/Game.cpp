#include "Game.hpp"
#include "Graphics/Window.hpp"
#include "Util.hpp"
#include <imgui_impl_sdl3.h>
#include <imgui_stdlib.h>
#include <print>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_render.h>
#include <thread>

void Game::initBeatmaps(const std::filesystem::path& path)
{
  if (!std::filesystem::exists(path))
    Util::panic("Unknown path '{}'", path.c_str());

  if (!std::filesystem::is_directory(path))
    Util::panic("'{}' is not a directory.", path.c_str());

  uint32_t i = 0;
  for (const auto& entry : std::filesystem::directory_iterator(path))
    if (entry.is_regular_file() && entry.path().extension() == ".osz")
    {
      Beatmap beatmap(entry.path());
      i += beatmap.getDifficulties().size();
      this->beatmaps.emplace_back(std::move(beatmap));
    }

  std::println("Finished loading {} beatmaps", i);
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
  static bool useVsync = true;
  if (ImGui::Checkbox("Use vsync", &useVsync))
    window->setVsync(useVsync);
  auto [trueWidth, trueHeight] = window->getTrueSize();
  ImGui::Text("True resolution: %dx%d", trueWidth, trueHeight);
  auto [scaledWidth, scaledHeight] = window->getScaledSize();
  ImGui::Text("Scaled resolution: %dx%d", scaledWidth, scaledHeight);
  ImGui::Text("Display density: %fx", double(trueWidth) / double(scaledWidth));
  ImGui::Text("Display scale (SDL): %fx", this->window->getDisplayScale());
  ImGui::End();

  ImGui::Begin("Beatmaps");
  static std::string searchText;
  ImGui::Text("Search:");
  ImGui::SameLine();
  ImGui::InputText("##", &searchText);
  if (this->activeBeatmap)
    ImGui::Text("Active beatmap: %s (%s)", this->activeBeatmap->title.c_str(), this->activeBeatmap->version.c_str());
  uint32_t i = 0;
  for (const Beatmap& beatmap : this->beatmaps)
    for (const BeatmapInfo& difficulty : beatmap.getDifficulties())
    {
      i++;
      if (!searchText.empty() && !difficulty.title.contains(searchText))
        continue;

      if (ImGui::Button(std::format("{} ({})##{}", difficulty.title.c_str(), difficulty.version.c_str(), i).c_str()))
        // TODO: Figure out why taking a pointer here results in garbage.
        this->activeBeatmap = difficulty;
    }
  ImGui::End();
}
