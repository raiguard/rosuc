#include "Window.hpp"
#include "Util.hpp"
#include <imgui_impl_sdlrenderer3.h>
#include <imgui_impl_sdl3.h>
#include <imgui.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>

Window::Window()
{
  // SDL_SetHint(SDL_HINT_EVENT_LOGGING, "2");
  if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0)
    Util::panic("Failed to initialize SDL: {}", SDL_GetError());

  this->sdlWindow = SDL_CreateWindow("Rai's osu!standard clone", 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
  if (!this->sdlWindow)
    Util::panic("Failed to create SDL window: {}", SDL_GetError());

  this->sdlRenderer = SDL_CreateRenderer(this->sdlWindow, nullptr);
  if (!this->sdlRenderer)
    Util::panic("Failed to create SDL renderer: {}", SDL_GetError());

  SDL_SetRenderVSync(this->sdlRenderer, 1);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui_ImplSDL3_InitForSDLRenderer(this->sdlWindow, this->sdlRenderer);
  ImGui_ImplSDLRenderer3_Init(this->sdlRenderer);
}

Window::~Window()
{
  ImGui_ImplSDLRenderer3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_DestroyRenderer(this->sdlRenderer);
  SDL_DestroyWindow(this->sdlWindow);
  SDL_Quit();
}

void Window::beginDrawing()
{
  ImGui_ImplSDLRenderer3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();
}

void Window::finishDrawing()
{
  ImGui::Render();
  SDL_RenderClear(this->sdlRenderer);
  ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), this->sdlRenderer);
  SDL_RenderPresent(this->sdlRenderer);
}

std::pair<int, int> Window::getTrueSize()
{
  int width, height;
  SDL_GetWindowSizeInPixels(this->sdlWindow, &width, &height);
  return {width, height};
}

std::pair<int, int> Window::getScaledSize()
{
  int width, height;
  SDL_GetWindowSize(this->sdlWindow, &width, &height);
  return {width, height};
}
