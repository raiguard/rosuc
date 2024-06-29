#include "Window.hpp"
#include "Util.hpp"
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>
#include <print>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_video.h>

Window::Window()
{
  // SDL_SetHint(SDL_HINT_EVENT_LOGGING, "2");
  if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0)
    Util::panic("Failed to initialize SDL: {}", SDL_GetError());

  SDL_GL_LoadLibrary(nullptr);

  this->sdlWindow = SDL_CreateWindow("Rai's osu!standard clone", 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_HIGH_PIXEL_DENSITY);
  if (!this->sdlWindow)
    Util::panic("Failed to create SDL window: {}", SDL_GetError());

  this->glContext = SDL_GL_CreateContext(this->sdlWindow);
  if (!this->glContext)
    Util::panic("Failed to create OpenGL context: {}", SDL_GetError());

  gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
  std::println("OpenGL version: {}", (const char*)(glGetString(GL_VERSION)));

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui_ImplSDL3_InitForOpenGL(this->sdlWindow, this->glContext);
  ImGui_ImplOpenGL3_Init();
}

Window::~Window()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL3_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(this->glContext);
  SDL_DestroyWindow(this->sdlWindow);
  SDL_Quit();
}

void Window::beginDrawing()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();
}

void Window::finishDrawing()
{
  ImGui::Render();
  auto dimensions = this->getTrueSize();
  glViewport(0, 0, dimensions.first, dimensions.second);
  glClearColor(0, 0, 0, 255);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  SDL_GL_SwapWindow(this->sdlWindow);
}

void Window::setVsync(bool value)
{
  SDL_GL_SetSwapInterval(value ? 1 : 0);
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
