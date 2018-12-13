//
//  UI.cpp
//  FinalProject
//
//  Created by Muyuan Chi on 2018/12/12.
//  Copyright © 2018年 Cain. All rights reserved.
//

#include "UI.h"

UI::UI(){
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  
  glVertexAttribPointer(0,
                        3,
                        GL_FLOAT, // What type these components are
                        GL_FALSE,
                        3 * sizeof(GLfloat),
                        (GLvoid*)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void UI::draw(GLuint shaderProgram){
  glUseProgram(shaderProgram);
  
  glBindVertexArray(VAO);
  glDrawArrays(GL_LINES, 0, 4);
}
