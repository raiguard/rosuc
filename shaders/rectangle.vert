#version 440 core

layout (location = 0) in vec2 vertex;

uniform mat4 model;
uniform mat4 world;

void main()
{
  gl_Position = world * model * vec4(vertex.xy, 0.0, 1.0);
}

