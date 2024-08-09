#version 440 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec4 tint;

void main()
{
  color = tint * texture(image, TexCoords);
}
