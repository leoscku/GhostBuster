//
//  Ghost.hpp
//  FinalProject
//
//  Created by Muyuan Chi on 2018/12/11.
//  Copyright © 2018年 Cain. All rights reserved.
//

#ifndef Ghost_hpp
#define Ghost_hpp

#include <stdio.h>
#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "Island.h"
class Island;
class Ghost{
public:
  Ghost(float x, float y, Island* island);
  Island* island;
  int hp = 3;
  glm::vec3 color = glm::vec3(0, 0, 0);
  glm::vec2 coord = glm::vec2(0, 0);
  glm::vec2 dir = glm::vec2(1, 0);
  glm::vec3 location = glm::vec3(0, 0, 0);
  void Move(double deltaTime);
  bool getHit(glm::vec3 position, glm::vec3 dir);
  void update();
  void draw(GLuint shaderProgram, glm::mat4 view);
  void changeDir();
  void init();
  void loadModel(char* filepath);
  void reorderData();
  void centerObject();
  GLuint VBO, VBO2, VAO, EBO;
  std::vector<glm::vec3> indices;
  std::vector<glm::vec3> vertices;
  std::vector<glm::vec3> normals;
  std::vector<glm::vec2> texiles;
  
  std::vector<GLuint> outIndices;
  std::vector<glm::vec3> outVertices;
  std::vector<glm::vec3> outNormals;
  std::vector<glm::vec2> outTexiles;
  
  void resizeObject();
};

#endif /* Ghost_hpp */
