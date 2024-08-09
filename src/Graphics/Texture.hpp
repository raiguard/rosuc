#pragma once
#include <filesystem>
#include <glad/glad.h>

class Texture
{
public:
  Texture(const std::filesystem::path& path);

  void bind();

  GLuint getID() const { return this->id; }

private:
  GLuint id;
};
