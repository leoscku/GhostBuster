#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in vec3 normalColor;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
  //color = vec4(1.0f, 0.41f, 0.7f, sampleExtraOutput);
  vec3 curr = vec3(normalColor.x, normalColor.y, normalColor.z);
  vec3 unitVector = normalize(curr);
  float unitX = (unitVector.x) * 0.5 + 0.5;
  float unitY = (unitVector.y) * 0.5 + 0.5;
  float unitZ = (unitVector.z) * 0.5 + 0.5;
  color = vec4(unitX, unitY, unitZ, 1.0f);
}
