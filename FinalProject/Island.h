//
//  island.hpp
//  FinalProject
//
//  Created by Muyuan Chi on 2018/11/27.
//  Copyright © 2018年 Cain. All rights reserved.
//

#ifndef Island_hpp
#define Island_hpp

#include <stdio.h>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "Window.h"
class Island
{
    public:
    Island();
    ~Island();
    
    std::vector<glm::vec3> coordData;
    std::vector<int> indices;
    
    glm::mat4 toWorld;
    
    int mapSize = 129;
    
    float mapArray[129][129];
    
    void generateRandomMap(glm::vec2 startPos );
    void mapSquare(int);
    void randomHeight(float);
    void getHeight(float, float);
    
    void squareStep(int, int, int);
    void diamondStep(int, int, int);
    
    void bufferData();
    
    void draw(GLuint shaderProgram);

    float random(int length);
    
    // These variables are needed for the shader program
    GLuint VBO, VAO, EBO;
    GLuint uProjection, uModelview;
};

#endif /* island_hpp */
