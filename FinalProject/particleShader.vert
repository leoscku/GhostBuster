#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 projection;
uniform vec3 offset;
uniform vec4 color;

uniform mat4 modelview;

out vec4 outputColor;

void main()
{
  float scale = 10.0f;
  outputColor = color;
  gl_Position = projection * modelview * vec4(position.x + offset.x, position.y + offset.y, position.z + offset.z, 1.0);
  
  //gl_Position = projection * vec4((vertex.xy * scale) + offset, 0.0, 1.0);
}
