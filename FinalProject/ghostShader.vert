#version 330 core
// NOTE: Do NOT use any version older than 330! Bad things will happen!

// This is an example vertex shader. GLSL is very similar to C.
// You can define extra functions if needed, and the main() function is
// called when the vertex shader gets run.
// The vertex shader gets called once per vertex.

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

// Uniform variables can be updated by fetching their location and passing values to that location
uniform mat4 projection;
uniform mat4 modelview;
uniform mat4 toWorld;
uniform mat3 model;

out vec3 fragNormalFix;
out vec3 fragPos;

void main()
{
  gl_Position = projection * modelview * vec4(position.x, position.y, position.z, 1.0);
  
  fragNormalFix = model * normal;
  fragPos = vec3(toWorld * vec4(position, 1.0f));
  
}
