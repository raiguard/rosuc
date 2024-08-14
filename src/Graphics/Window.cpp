#include "Window.hpp"
#include "Util.hpp"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl2.h>
#include <print>
#include <SDL.h>
#include <SDL_image.h>

Window::Window()
{
  // SDL_SetHint(SDL_HINT_EVENT_LOGGING, "2");
  if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0)
    Util::panic("Failed to initialize SDL: {}", SDL_GetError());

  SDL_GL_LoadLibrary(nullptr);

  this->sdlWindow = SDL_CreateWindow("Rai's osu!standard clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  if (!this->sdlWindow)
    Util::panic("Failed to create SDL window: {}", SDL_GetError());

  this->glContext = SDL_GL_CreateContext(this->sdlWindow);
  if (!this->glContext)
    Util::panic("Failed to create OpenGL context: {}", SDL_GetError());

  gladLoadGLLoader(SDL_GL_GetProcAddress);
  std::println("OpenGL version: {}", (const char*)(glGetString(GL_VERSION)));

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();

  ImGui_ImplSDL2_InitForOpenGL(this->sdlWindow, this->glContext);
  ImGui_ImplOpenGL3_Init();

  IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 4);
  glEnable(GL_MULTISAMPLE);

  {
    this->spriteShader = std::make_unique<Shader>("shaders/sprite.vert", "shaders/sprite.frag");
    glGenBuffers(1, &this->spriteVBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->spriteVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->spriteVertices), this->spriteVertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &this->spriteVAO);
    glBindVertexArray(this->spriteVAO);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
  }
  this->hitcircleTexture = std::make_unique<Texture>("assets/hitcircle.png");
  this->hitcircleOverlayTexture = std::make_unique<Texture>("assets/hitcircleoverlay.png");
  this->hitcircle1 = std::make_unique<Texture>("assets/default-1.png");

  SDL_Surface* cursorSurface = IMG_Load("assets/cursor.png");
  this->cursor = SDL_CreateColorCursor(cursorSurface, cursorSurface->w / 2, cursorSurface->h / 2);
  if (!this->cursor)
    Util::panic("Failed to create cursor: {}", SDL_GetError());
  SDL_FreeSurface(cursorSurface);
}

Window::~Window()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_FreeCursor(this->cursor);

  SDL_GL_DeleteContext(this->glContext);
  SDL_DestroyWindow(this->sdlWindow);
  IMG_Quit();
  SDL_Quit();
}

void Window::beginDrawing()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();
}

void Window::finishDrawing()
{
  ImGui::Render();
  auto [width, height] = this->getTrueSize();
  glViewport(0, 0, width, height);
  glClearColor(0, 0, 0, 255);
  glClear(GL_COLOR_BUFFER_BIT);

  glm::mat4 world = glm::ortho(0.0f, float(width), float(height), 0.0f, -1.0f, 1.0f);
  this->spriteShader->bind();
  this->spriteShader->setMat4("world", world);

  for (float i = 300.0f; i < 600.0f; i += 45.2f)
  {
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(i, i, 0.0f));
    model = glm::scale(model, glm::vec3(128.0f, 128.0f, 1.0f));
    this->spriteShader->setMat4("model", model);
    this->spriteShader->setVec4("tint", 0.102f, 0.455f, 0.949f, 1.0f);
    this->hitcircleTexture->bind();
    glBindVertexArray(this->spriteVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    this->hitcircleOverlayTexture->bind();
    this->spriteShader->setVec4("tint", 1.0f, 1.0f, 1.0f, 1.0f);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    this->hitcircle1->bind();
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(i, i, 0.0f));
    model = glm::scale(model, glm::vec3(41.0f, 60.0f, 1.0f));
    this->spriteShader->setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  SDL_SetCursor(this->cursor);
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
