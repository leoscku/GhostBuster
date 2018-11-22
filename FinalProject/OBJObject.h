#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#ifdef __APPLE__
#include <iostream>
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
//#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <random>

class OBJObject
{
private:
    std::vector<glm::vec3> indices;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texiles;
    
    std::vector<GLuint> outIndices;
    std::vector<glm::vec3> outVertices;
    std::vector<glm::vec3> outNormals;
    std::vector<glm::vec2> outTexiles;
    
    // These variables are needed for the shader program
    GLuint VBO, VBO2, VBO3, VAO, EBO;
    GLuint uProjection, uModelview, uTex;
    GLuint textureID; 
    
    void parse(const char* filepath);
    
    unsigned char* loadPPM(const char* filename, int& width, int& height); 
    
    void reorderData(); 

public:
	OBJObject(const char* OBJfilepath, const char* TEXfilepath);
    ~OBJObject();
    glm::mat4 toWorld;
    
    float angle;

    void draw(GLuint shaderProgram);
    
    void draw(GLuint shaderProgram, glm::mat4 toWorld);
        
    void update();
        
    void centerObject();
    
    void scale(float coefficient);
    
    void setPosition(glm::vec3);
    
    void loadTexture(const char* filepath);
    
};

#endif
