#ifndef BEZIER_H
#define BEZIER_H

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
#include "Window.h"

#include "Node.h"

class Bezier: public Node
{
private:
    
    GLfloat length = 5.0f;
  
    bool dir_up = false; 
    
    std::vector<glm::vec3> points = {
        {-0.5f, 0.0f, -0.5f},
        {-0.33f, 0.05f, -0.5f},
        {0.33f, -0.05f, -0.5f},
        {0.5f, 0.0f, -0.5f},
        
        {-0.5f, 0.0f, -0.33f},
        {-0.33f, 0.05f, -0.33f},
        {0.33f, -0.05f, -0.33f},
        {0.5f, 0.0f, -0.33f},
        
        {-0.5f, 0.0f, 0.33f},
        {-0.33f, 0.05f, 0.33f},
        {0.33f, -0.05f, 0.33f},
        {0.5f, 0.0f, 0.33f},
        
        {-0.5f, 0.0f, 0.5f},
        {-0.33f, 0.05f, 0.5f},
        {0.33f, -0.05f, 0.5f},
        {0.5f, 0.0f, 0.5f}
        /*{-0.5f, 0.0f, -0.5f},
        {-0.33f, 0.0f, -0.5f},
        {0.33f, -0.0f, -0.5f},
        {0.5f, 0.0f, -0.5f},
        
        {-0.5f, 0.0f, -0.33f},
        {-0.33f, 0.0f, -0.33f},
        {0.33f, -0.0f, -0.33f},
        {0.5f, 0.0f, -0.33f},
        
        {-0.5f, 0.0f, 0.33f},
        {-0.33f, 0.0f, 0.33f},
        {0.33f, -0.0f, 0.33f},
        {0.5f, 0.0f, 0.33f},
        
        {-0.5f, 0.0f, 0.5f},
        {-0.33f, 0.0f, 0.5f},
        {0.33f, -0.0f, 0.5f},
        {0.5f, 0.0f, 0.5f}*/
    };
    
    std::vector<glm::vec3> controlPoints; 
    
    std::vector<glm::vec3> controlLines;
    // These variables are needed for the shader program
    GLuint VBO, VBO2, VBO3, VBO4, VAO, VAO2, VAO3, VAO4, EBO;
    GLuint uProjection, uModelview, uModel, uModelFix, uCameraPos;
    
    void calculateVerticesIndices(); 
    
    
public:
	Bezier();
    ~Bezier();
    glm::mat4 toWorld;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<GLuint> indices;
    GLfloat waveHeight = 0.01f;
    
    void draw(int program, glm::mat4 M);
    
    void scale(float coefficient);
    
    void update();
        
};

#endif
