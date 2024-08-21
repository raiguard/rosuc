#include "Graphics/Shader.hpp"
#include "Util.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::filesystem::path& vertPath, const std::filesystem::path& fragPath)
{
  uint32_t vert = Shader::compile(GL_VERTEX_SHADER, vertPath);
  uint32_t frag = Shader::compile(GL_FRAGMENT_SHADER, fragPath);
  this->id = glCreateProgram();
  glAttachShader(this->id, vert);
  glAttachShader(this->id, frag);
  glLinkProgram(this->id);
  glValidateProgram(this->id);
  glDeleteShader(vert);
  glDeleteShader(frag);
}

uint32_t Shader::compile(GLenum type, const std::filesystem::path& path)
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

void Shader::bind()
{
  glUseProgram(this->id);
}

void Shader::setBool(const std::string& name, bool value) const
{
  glUniform1i(glGetUniformLocation(this->id, name.c_str()), (int32_t)value);
}

void Shader::setInt(const std::string& name, int32_t value) const
{
  glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
  glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, glm::mat4 value) const
{
  glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setVec2(const std::string& name, float x, float y) const
{
  glUniform2f(glGetUniformLocation(this->id, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
  glUniform3f(glGetUniformLocation(this->id, name.c_str()), x, y, z);
}

void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
  glUniform3fv(glGetUniformLocation(this->id, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
  glUniform4f(glGetUniformLocation(this->id, name.c_str()), x, y, z, w);
}

void Shader::setVec4(const std::string& name, glm::vec4 value) const
{
  glUniform4fv(glGetUniformLocation(this->id, name.c_str()), 1, glm::value_ptr(value));
}

