//
//  UI.hpp
//  FinalProject
//
//  Created by Muyuan Chi on 2018/12/12.
//  Copyright © 2018年 Cain. All rights reserved.
//

#ifndef UI_hpp
#define UI_hpp

#include <stdio.h>

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

class UI{
public:
  const GLfloat vertices[4][3] = {
    {-0.025f, 0,  0}, {0.025f, 0,  0}, {0, -0.05f,  0}, {0, 0.05f,  0}
  };
  
  const int indices[4] = {0, 1, 2, 3};
  GLuint VAO, VBO, EBO;
  
  UI();
  
  void draw(GLuint shaderProgram);
};
#endif /* UI_hpp */
