#version 440 core

layout (location = 0) in vec4 position;

uniform mat4 model;
uniform mat4 world;

void main()
{
  gl_Position = world * model * position;
}
