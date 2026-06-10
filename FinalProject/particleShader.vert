#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform vec3 offset;
uniform vec4 color;

uniform mat4 modelview;
uniform float pscale;

out vec4 outputColor;

void main()
{
  outputColor = color;
  vec3 p = position * pscale;
  gl_Position = projection * modelview * vec4(p.x + offset.x, p.y + offset.y, p.z + offset.z, 1.0);
}
