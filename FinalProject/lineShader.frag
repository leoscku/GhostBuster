#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
// in vec2 fragTexCoord;
//uniform sampler2D tex;
// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

in vec3 fragNormal;
in vec3 fragPosition;

uniform samplerCube skybox;
uniform vec3 cameraPos;

float calculateFresnel() {
    vec3 viewVector = normalize(fragPosition - cameraPos);
    vec3 normal = normalize(fragNormal);
    float refractiveFactor = dot(viewVector, normal);
    refractiveFactor = pow(refractiveFactor, 0.5f);
    return clamp(refractiveFactor, 0.0f, 1.0f);
}

void main()
{
    vec3 I = normalize(fragPosition - cameraPos);
    vec3 R = reflect(I, normalize(fragNormal));
    vec4 reflectColor = vec4(texture(skybox, R).rgb, 1.0);
    
    float ratio = 1.00 / 1.33;
    vec3 Rf = refract(I, -normalize(fragNormal), ratio);
    vec4 refractColor = vec4(texture(skybox, Rf).rgb, 1.0);
    
    // color = reflectColor*0.15 + refractColor*0.85;
    color = mix(reflectColor, refractColor, calculateFresnel());
}

