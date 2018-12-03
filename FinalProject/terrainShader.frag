#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
uniform sampler2D tex; // this is the texture
in vec2 fragTexCoord;
in vec3 fragNormal;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

in vec3 pos;
void main()
{
    // Color everything a hot pink color. An alpha of 1.0f means it is not transparent.
    //color = vec4( pos.y / (133 * 2) + 0.5, pos.y / (133 * 2) + 0.5, pos.y / (133 * 2) + 0.5, sampleExtraOutput);
    //color = vec4(texture(tex, fragTexCoord).rgb,1.0);//vec2(0.5, 0.5));// vec4(1.0f, 0.41f, 0.7f, sampleExtraOutput);
    
    color = vec4(vec3(fragNormal * 0.5f) * vec3(1, 1, 0), 1.0);
}
