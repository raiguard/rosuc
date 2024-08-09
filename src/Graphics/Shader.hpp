#pragma once
#include <filesystem>
#include <glad/glad.h>
#include <glm/fwd.hpp>

class Shader
{
public:
  Shader(const std::filesystem::path& vert, const std::filesystem::path& frag);

  void bind();

  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int32_t value) const;
  void setFloat(const std::string& name, float value) const;
  void setMat4(const std::string& name, glm::mat4 value) const;
  void setVec3(const std::string& name, float x, float y, float z) const;
  void setVec3(const std::string& name, glm::vec3 value) const;
  void setVec4(const std::string& name, float x, float y, float z, float w) const;
  void setVec4(const std::string& name, glm::vec4 value) const;

  uint32_t getID() const { return this->id; }

private:
  static uint32_t compile(GLenum type, const std::filesystem::path& path);
  uint32_t id;
};
