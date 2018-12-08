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
    glm::vec3 normals[1025][1025];
    std::vector<glm::vec2> texture;

    glm::mat4 toWorld;
    
    int mapSize = 1025;
    float squareSize = 2.2f;
    
    float mapArray[1025][1025];
    
    void generateRandomMap(glm::vec2 startPos );
    void mapSquare(int);
    void randomHeight(float);
    void getHeight(float, float);
    
    void squareStep(int, int, int);
    void diamondStep(int, int, int);
    
    void bufferData();
    
    void draw(GLuint shaderProgram, glm::mat4 view);
    float getY(glm::vec2 coord);

    float random(int length);
    
    void reGenerateData();
    
    int loadTexture();
    
    int texID;
    
    bool inMap(glm::vec2 coord);
    
    // These variables are needed for the shader program
    GLuint VBO, VAO, EBO, CBO, VBO2;
    GLuint uProjection, uModelview, uViewPos;
};

#endif /* island_hpp */
