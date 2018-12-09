#version 330 core

in vec4 outputColor;

out vec4 color;


void main()
{
  // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
  //color = vec4(1.0f, 0.41f, 0.7f, sampleExtraOutput);
//  vec3 curr = vec3(normalColor.x, normalColor.y, normalColor.z);
//  vec3 unitVector = normalize(curr);
//  float unitX = (unitVector.x) * 0.5 + 0.5;
//  float unitY = (unitVector.y) * 0.5 + 0.5;
//  float unitZ = (unitVector.z) * 0.5 + 0.5;
//  
  color = outputColor;
  
}
