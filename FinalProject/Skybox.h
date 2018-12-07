#ifndef _SKYCUBE_H_
#define _SKYCUBE_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Skybox
{
public:
    Skybox();
    ~Skybox(); 
    
    glm::mat4 toWorld;
    
    void draw(GLuint);
    void update();
    void loadSkyboxTextures();
    
    // These variables are needed for the shader program
    GLuint VBO, VAO, EBO;
    GLuint uProjection, uModelview;
    GLuint skyTextureID;
    
    // Define the coordinates and indices needed to draw the cube. Note that it is not necessary
    // to use a 2-dimensional array, since the layout in memory is the same as a 1-dimensional array.
    // This just looks nicer since it's easy to tell what coordinates/indices belong where.
    float length = 600.0f;
    float vertices[108] = {
        // positions
        -length,  length, -length,
        -length, -length, -length,
        length, -length, -length,
        length, -length, -length,
        length,  length, -length,
        -length,  length, -length,
        
        -length, -length,  length,
        -length, -length, -length,
        -length,  length, -length,
        -length,  length, -length,
        -length,  length,  length,
        -length, -length,  length,
        
        length, -length, -length,
        length, -length,  length,
        length,  length,  length,
        length,  length,  length,
        length,  length, -length,
        length, -length, -length,
        
        -length, -length,  length,
        -length,  length,  length,
        length,  length,  length,
        length,  length,  length,
        length, -length,  length,
        -length, -length,  length,
        
        -length,  length, -length,
        length,  length, -length,
        length,  length,  length,
        length,  length,  length,
        -length,  length,  length,
        -length,  length, -length,
        
        -length, -length, -length,
        -length, -length,  length,
        length, -length, -length,
        length, -length, -length,
        -length, -length,  length,
        length, -length,  length
    };
};

#endif


