#include "Game.hpp"
#include "Graphics/Window.hpp"
#include <imgui_impl_sdl2.h>
#include <imgui_stdlib.h>
#include <print>
#include <SDL_audio.h>
#include <SDL_events.h>
#include <SDL_render.h>
#include <thread>
#include "Util.hpp"

void Game::initBeatmaps(const std::filesystem::path& path)
{
  if (!std::filesystem::exists(path))
  {
    std::println("Cannot load beatmaps at '{}': directory does not exist.", path.c_str());
    return;
  }

  if (!std::filesystem::is_directory(path))
    Util::panic("'{}' is not a directory.", path.c_str());

  uint32_t i = 0;
  for (const auto& entry : std::filesystem::directory_iterator(path))
    if (entry.is_directory())
    {
      BeatmapSet beatmap(entry.path());
      if (beatmap.getDifficulties().empty())
        continue;
      i += beatmap.getDifficulties().size();
      this->beatmaps.emplace_back(std::move(beatmap));
    }

  std::println("Loaded {} beatmaps", i);

  std::sort(this->beatmaps.begin(), this->beatmaps.end(), [](const BeatmapSet& a, const BeatmapSet& b)
  {
    return a.getDifficulties()[0].title < b.getDifficulties()[0].title;
  });
}

void Game::init()
{
  this->window.reset(new Window());
  this->audioManager.reset(new AudioManager());
}

Game::ShouldQuit Game::frame()
{
  using Clock = std::chrono::high_resolution_clock;
  using namespace std::chrono_literals;

  static Clock::time_point lastFrameTime = Clock::now();
  static Clock::duration accumulator;
  static constexpr std::chrono::nanoseconds timestep(1000000000 / Game::UPS);

  Clock::time_point currentTime = Clock::now();
  Clock::duration frameTime = currentTime - lastFrameTime;
  lastFrameTime = currentTime;

  accumulator += frameTime;

  // TODO: Handle input or rendering on a separate thread to make input precision FPS-agnostic
  if (this->handleEvents())
    return ShouldQuit::True;

  while (accumulator >= timestep)
  {
    this->update();
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

  return ShouldQuit::False;
}

Game::ShouldQuit Game::handleEvents()
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    if (ImGui_ImplSDL2_ProcessEvent(&event))
      continue;
    if (event.type == SDL_QUIT)
      return ShouldQuit::True;
  }

  return ShouldQuit::False;
}

Game::ShouldQuit Game::update()
{
  this->audioManager->update();
  return ShouldQuit::False;
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
  ImGui::Separator();
  ImGui::Text("Audio driver: %s", SDL_GetCurrentAudioDriver());
  ImGui::End();

  ImGui::Begin("Beatmaps");
  static std::string searchText;
  ImGui::Text("Search:");
  ImGui::SameLine();
  ImGui::InputText("##", &searchText);
  if (this->activeBeatmap)
    ImGui::Text("Active beatmap: %s (%s)", this->activeBeatmap->getInfo().title.c_str(), this->activeBeatmap->getInfo().version.c_str());
  ImGui::BeginChild("##");
  uint32_t i = 0;
  for (const BeatmapSet& beatmapSet : this->beatmaps)
    for (const Beatmap& beatmap : beatmapSet.getDifficulties())
    {
      i++;
      if (!searchText.empty() && !Util::icontains(beatmap.title, searchText))
        continue;

      if (ImGui::Button(std::format("{} ({})##{}", beatmap.title.c_str(), beatmap.version.c_str(), i).c_str()))
      {
        this->activeBeatmap = std::make_unique<ActiveBeatmapInfo>(beatmap);
        this->audioManager->playSong(this->activeBeatmap->getInfo().audio, this->activeBeatmap->getInfo().previewTime, AudioManager::Repeat::True);
      }
    }
  ImGui::EndChild();
  ImGui::End();

  ImGui::ShowDemoWindow();
}
