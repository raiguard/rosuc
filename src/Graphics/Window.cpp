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

  this->sdlWindow = SDL_CreateWindow("Rai's osu!standard clone",
                                     SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
                                     SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
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

  {
    this->spriteShader = std::make_unique<Shader>("shaders/sprite.vert", "shaders/sprite.frag");
    glGenBuffers(1, &this->spriteVBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->spriteVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->spriteVertices), this->spriteVertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &this->spriteVAO);
    glBindVertexArray(this->spriteVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  }
  {
    this->rectShader = std::make_unique<Shader>("shaders/rectangle.vert", "shaders/rectangle.frag");
    glGenBuffers(1, &this->rectVBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(this->rectVertices), this->rectVertices, GL_STATIC_DRAW);
    glGenVertexArrays(1, &this->rectVAO);
    glBindVertexArray(this->rectVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
  }
  this->hitcircleTexture = std::make_unique<Texture>("assets/hitcircle.png");
  this->hitcircleOverlayTexture = std::make_unique<Texture>("assets/hitcircleoverlay.png");
  this->hitcircle0 = std::make_unique<Texture>("assets/default-0.png");
  this->hitcircle1 = std::make_unique<Texture>("assets/default-1.png");
  this->hitcircle2 = std::make_unique<Texture>("assets/default-2.png");
  this->hitcircle3 = std::make_unique<Texture>("assets/default-3.png");
  this->hitcircle4 = std::make_unique<Texture>("assets/default-4.png");
  this->hitcircle5 = std::make_unique<Texture>("assets/default-5.png");
  this->hitcircle6 = std::make_unique<Texture>("assets/default-6.png");
  this->hitcircle7 = std::make_unique<Texture>("assets/default-7.png");
  this->hitcircle8 = std::make_unique<Texture>("assets/default-8.png");
  this->hitcircle9 = std::make_unique<Texture>("assets/default-9.png");

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
  this->rectShader->bind();
  this->rectShader->setMat4("world", world);

  const std::array<Texture*, 10> numberTextures = {
    this->hitcircle0.get(),
    this->hitcircle1.get(),
    this->hitcircle2.get(),
    this->hitcircle3.get(),
    this->hitcircle4.get(),
    this->hitcircle5.get(),
    this->hitcircle6.get(),
    this->hitcircle7.get(),
    this->hitcircle8.get(),
    this->hitcircle9.get(),
  };
  int j = 8;
  for (float i = 600.0f; i >= 300.0f; i -= 50.0f)
  {
    j--;
    float alpha = 300.0f / (i + ((i - 300.0f) * 2));
    this->drawSprite(this->hitcircleTexture.get(), i, i, 128.0f, 128.0f, glm::vec4(0.102f, 0.455f, 0.949f, alpha));
    this->drawSprite(this->hitcircleOverlayTexture.get(), i, i, 128.0f, 128.0f, glm::vec4(1.0f, 1.0f, 1.0f, alpha));
    this->drawSprite(numberTextures[j], i, i, float(this->hitcircle1->getWidth()) * 0.8f, float(this->hitcircle1->getHeight()) * 0.8f, glm::vec4(1.0f, 1.0f, 1.0f, alpha));
  }

  {
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(100.0f, 100.0f, 0.0f));
    model = glm::scale(model, glm::vec3(128.0f, 128.0f, 1.0f));
    this->rectShader->bind();
    this->rectShader->setMat4("model", model);
    this->rectShader->setVec4("inColor", 1.0f, 0.0f, 0.0f, 1.0f);
    glBindVertexArray(this->rectVAO);
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

void Window::drawSprite(Texture* texture, float x, float y, float width, float height, glm::vec4 tint)
{
  glm::mat4 model(1.0f);
  model = glm::translate(model, glm::vec3(x, y, 0.0f));
  model = glm::scale(model, glm::vec3(width, height, 1.0f));
  this->spriteShader->bind();
  this->spriteShader->setMat4("model", model);
  this->spriteShader->setVec4("tint", tint);

  glActiveTexture(GL_TEXTURE0);
  texture->bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindVertexArray(this->spriteVAO);
  glDrawArrays(GL_TRIANGLES, 0, 6);
}
