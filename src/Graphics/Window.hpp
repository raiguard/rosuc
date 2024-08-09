#pragma once
#include "Graphics/Shader.hpp"
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

  std::unique_ptr<Shader> spriteShader;
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
  uint32_t hitcircleOverlayTextureID;
};
