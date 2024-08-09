#version 440 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D image;
uniform vec3 tint;

void main()
{
  vec4 texColor = texture(image, TexCoords);
  if (texColor.a < 0.1)
    discard;
  color = vec4(tint, 1.0) * texColor;
}
