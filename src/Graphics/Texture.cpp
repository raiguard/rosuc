#include "Graphics/Texture.hpp"
#include "Util.hpp"
#include <SDL_image.h>
#include <SDL_surface.h>

Texture::Texture(const std::filesystem::path& path)
{
  SDL_Surface* surface = IMG_Load(path.c_str());
  if (!surface)
    Util::panic("Failed to load texture {}: {}", path.c_str(), SDL_GetError());
  glGenTextures(1, &this->id);
  glBindTexture(GL_TEXTURE_2D, this->id);
  // TODO: Ensure that it's RGBA8888
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
  glGenerateMipmap(GL_TEXTURE_2D);
  this->width = surface->w;
  this->height = surface->h;
  SDL_FreeSurface(surface);
}

void Texture::bind()
{
  glBindTexture(GL_TEXTURE_2D, this->id);
}
