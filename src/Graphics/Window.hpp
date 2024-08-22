#pragma once
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"
#include <glm/glm.hpp>
#include <SDL_mouse.h>
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

  void drawSprite(Texture* texture,
                  float x, float y,
                  float width, float height,
                  glm::vec4 tint = glm::vec4(1.0f));

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
    -0.5f, 0.5f, 0.0f, 1.0f,
    0.5f, -0.5f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 0.0f,

    -0.5f, 0.5f, 0.0f, 1.0f,
    0.5f, 0.5f, 1.0f, 1.0f,
    0.5f, -0.5f, 1.0f, 0.0f,
  };

  std::unique_ptr<Shader> rectShader;
  uint32_t rectVBO;
  uint32_t rectVAO;
  static constexpr float rectVertices[12] = {
    // pos
    -0.5f, 0.5f,
    0.5f, -0.5f,
    -0.5f, -0.5f,

    -0.5f, 0.5f,
    0.5f, 0.5f,
    0.5f, -0.5f,
  };

  std::unique_ptr<Texture> hitcircleTexture;
  std::unique_ptr<Texture> hitcircleOverlayTexture;
  std::unique_ptr<Texture> hitcircle0;
  std::unique_ptr<Texture> hitcircle1;
  std::unique_ptr<Texture> hitcircle2;
  std::unique_ptr<Texture> hitcircle3;
  std::unique_ptr<Texture> hitcircle4;
  std::unique_ptr<Texture> hitcircle5;
  std::unique_ptr<Texture> hitcircle6;
  std::unique_ptr<Texture> hitcircle7;
  std::unique_ptr<Texture> hitcircle8;
  std::unique_ptr<Texture> hitcircle9;
  SDL_Cursor* cursor;
};
