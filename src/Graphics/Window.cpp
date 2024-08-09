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

uint32_t compileShader(GLenum type, const std::filesystem::path& path)
{
  uint32_t id = glCreateShader(type);
  std::string source = Util::readFile(path);
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result)
    return id;

  int length;
  glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
  std::string message;
  message.resize(length);
  glGetShaderInfoLog(id, length, &length, message.data());
  Util::panic("Failed to compile shader {}:\n\t{}", path.c_str(), message);
}

uint32_t createShaderProgram(const std::string& name)
{
  uint32_t program = glCreateProgram();
  uint32_t vertex = compileShader(GL_VERTEX_SHADER, "shaders/" + name + ".vert");
  uint32_t fragment = compileShader(GL_FRAGMENT_SHADER, "shaders/" + name + ".frag");
  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);
  glValidateProgram(program);
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  return program;
}

Window::Window()
{
  // SDL_SetHint(SDL_HINT_EVENT_LOGGING, "2");
  if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) != 0)
    Util::panic("Failed to initialize SDL: {}", SDL_GetError());

  SDL_GL_LoadLibrary(nullptr);

  this->sdlWindow = SDL_CreateWindow("Rai's osu!standard clone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
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

  ImGui_ImplSDL2_InitForOpenGL(this->sdlWindow, this->glContext);
  ImGui_ImplOpenGL3_Init();

  this->triangleShader = createShaderProgram("triangle");

  glGenBuffers(1, &this->triangleVBO);
  glBindBuffer(GL_ARRAY_BUFFER, this->triangleVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(this->triangleVertices), this->triangleVertices, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
}

Window::~Window()
{
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();

  SDL_GL_DeleteContext(this->glContext);
  SDL_DestroyWindow(this->sdlWindow);
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

  glUseProgram(this->triangleShader);
  glm::mat4 model(1.0f);
  model = glm::translate(model, glm::vec3(100.0f, 100.0f, 0.0f));
  model = glm::scale(model, glm::vec3(20.0f, 20.0f, 1.0f));
  glUniformMatrix4fv(glGetUniformLocation(this->triangleShader, "model"), 1, GL_FALSE, glm::value_ptr(model));

  glm::mat4 world = glm::ortho(0.0f, float(width), float(height), 0.0f, -1.0f, 1.0f);
  glUniformMatrix4fv(glGetUniformLocation(this->triangleShader, "world"), 1, GL_FALSE, glm::value_ptr(world));

  glDrawArrays(GL_TRIANGLES, 0, 3);

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
