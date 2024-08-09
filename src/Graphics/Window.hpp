#pragma once
#include <SDL_video.h>
#include <utility>

class Window
{
public:
  Window();
  ~Window();

  void beginDrawing();
  void extracted();
  void finishDrawing();

  void setVsync(bool value);

  void drawDebugGui();

  std::pair<int, int> getTrueSize();
  std::pair<int, int> getScaledSize();

  void testGraphics();

private:
  SDL_Window* sdlWindow;
  SDL_GLContext glContext;

  uint32_t triangleShader;
  uint32_t triangleVBO;
  uint32_t triangleVAO;
  static constexpr float triangleVertices[6] = {
    -1.0f, 1.0f,
    1.0f, 1.0f,
    0.0f, -1.0f,
  };

  uint32_t spriteShader;
  uint32_t spriteVBO;
  uint32_t spriteVAO;
  static constexpr float spriteVertices[24] = {
    // pos      // tex
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
  };


  uint32_t hitcircleTextureID;
};
