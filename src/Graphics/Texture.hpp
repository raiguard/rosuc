#pragma once
#include <filesystem>
#include <glad/glad.h>

class Texture
{
public:
  Texture(const std::filesystem::path& path);

  void bind();

  GLuint getID() const { return this->id; }

  int getWidth() const { return this->width; }
  int getHeight() const { return this->height; }

private:
  GLuint id;
  int width;
  int height;
};
