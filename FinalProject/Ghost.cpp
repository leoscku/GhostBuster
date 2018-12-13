//
//  Ghost.cpp
//  FinalProject
//
//  Created by Muyuan Chi on 2018/12/11.
//  Copyright © 2018年 Cain. All rights reserved.
//

#include "Ghost.h"

Ghost::Ghost(float x, float y, Island* island){
  this->island = island;
  this->location = glm::vec3(x,island->getY(glm::vec2(x, y)) ,y);
  this->coord = glm::vec2(x, y);
  this->color = glm::vec3(0);
  init();
  changeDir();
}

void Ghost::loadModel(char* filepath){
  FILE* fp;
  float x, y, z;
  float r, g, b;
  int c1, c2;
  float tx, ty;
  int f_v1, f_t1, f_n1, f_v2, f_t2, f_n2, f_v3, f_t3, f_n3, f_v4, f_t4, f_n4;
  
  fp = fopen(filepath, "rb");
  if (fp==NULL) {std::cerr << "Error loading file" << std::endl; exit(-1); }
  while (1) {
    c1 = fgetc(fp);
    c2 = fgetc(fp);
    int error;
    
    while (c1 == '\n') {
      c1 = c2;
      c2 = fgetc(fp);
    }
    
    if (c1 == 'v' && c2 == ' ') {
      error = fscanf(fp, "%f %f %f %f %f %f\n", &x, &y, &z);
      if (error != EOF) {
        this->vertices.push_back(glm::vec3(x, y, z));
      }
    } else if (c1 == 'v' && c2 == 'n') {
      error = fscanf(fp, "%f %f %f\n", &x, &y, &z);
      if (error != EOF) {
        this->normals.push_back(glm::normalize(glm::vec3(x, y, z)));
      }
    } else if (c1 == 'v' && c2 == 't') {
      error = fscanf(fp, "%f %f\n", &tx, &ty);
      if (error != EOF) {
        this->texiles.push_back(glm::vec2(tx, ty));
      }
    } else if (c1 == 'f' && c2 == ' ') {
      error = fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &f_v1, &f_t1, &f_n1, &f_v2, &f_t2, &f_n2, &f_v3, &f_t3, &f_n3);
      if (error != EOF) {
        this->indices.push_back(glm::vec3(f_v1, f_t1, f_n1));
        this->indices.push_back(glm::vec3(f_v2, f_t2, f_n2));
        this->indices.push_back(glm::vec3(f_v3, f_t3, f_n3));
      }
    }
    if (feof(fp)) break;
  }
  std::cout << "faces: " << this->indices.size() << std::endl;
  std::cout << "vetices: " << this->vertices.size() << std::endl;
  std::cout << "normals: " << this->normals.size() << std::endl;
  std::cout << "texiles: " << this->texiles.size() << std::endl;
  
  /*for (int i = 0; i < texiles.size(); i++) {
   std::cout << texiles[i].x << " " << texiles[i].y << std::endl;
   }*/
  fclose(fp);
}

void Ghost::init(){
  loadModel("boo.obj");
  resizeObject();
  reorderData();
  
  std::cout << "faces: " << this->outIndices.size() << std::endl;
  std::cout << "vetices: " << this->outVertices.size() << std::endl;
  std::cout << "normals: " << this->outNormals.size() << std::endl;
  std::cout << "texiles: " << this->outTexiles.size() << std::endl;
  // Create array object and buffers. Remember to delete your buffers when the object is destroyed!
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &VBO2);
  glGenBuffers(1, &EBO);
  
  // Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
  // Consider the VAO as a container for all your buffers.
  glBindVertexArray(VAO);
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, outVertices.size() * sizeof(GLfloat) * 3, &outVertices[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,
                        3,
                        GL_FLOAT, // What type these components are
                        GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                        3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                        (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
  
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, outNormals.size() * sizeof(GLfloat) * 3, &outNormals[0], GL_STATIC_DRAW);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1,
                        3,
                        GL_FLOAT, // What type these components are
                        GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                        3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                        (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
  
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, outIndices.size() * sizeof(GLuint), &outIndices[0], GL_STATIC_DRAW);
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void Ghost::reorderData(){
  for (unsigned int i = 0; i < indices.size(); i++) {
    int vi = indices[i].x;
    int ti = indices[i].y;
    int ni = indices[i].z;
    
    outVertices.push_back(vertices[vi-1]);
    outTexiles.push_back(texiles[ti-1]);
    outNormals.push_back(normals[ni-1]);
    outIndices.push_back(i);
  }
}

void Ghost::draw(GLuint shaderProgram, glm::mat4 view){
  if(hp <= 0){
    return;
  }
  glUseProgram(shaderProgram);
  float theta = acos(glm::dot(glm::vec2(0, 1),dir));
  if (glm::cross(glm::vec3(0, 0, 1), glm::vec3(dir.x,0, dir.y)).y < 0){
    theta = 2 * 3.1415926f - theta;
  }
  glm::mat4 toWorld = glm::translate(glm::mat4(1.0f), location) * glm::rotate(glm::mat4(1.0f),  theta, glm::vec3(0,1,0));
  glm::mat4 modelview = view * toWorld;
  glm::mat3 model = glm::mat3(glm::transpose(glm::inverse(toWorld)));
  
  GLuint uToWorld = glGetUniformLocation(shaderProgram, "toWorld");
  GLuint uModel = glGetUniformLocation(shaderProgram, "model");
  GLuint uViewPos = glGetUniformLocation(shaderProgram, "viewPos");
  GLuint uProjection = glGetUniformLocation(shaderProgram, "projection");
  GLuint uModelview = glGetUniformLocation(shaderProgram, "modelview");
  GLuint uToon = glGetUniformLocation(shaderProgram, "toon");
  
  
  // Now send these values to the shader program
  glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
  glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
  glUniformMatrix4fv(uToWorld, 1, GL_FALSE, &toWorld[0][0]);
  glUniform3f(uViewPos, Window::cam_pos.x, Window::cam_pos.y, Window::cam_pos.z);
  glUniformMatrix3fv(uModel, 1, GL_FALSE, &model[0][0]);
  glUniform1i(uToon, Window::toon); 

  
  // Now draw the object. We simply need to bind the VAO associated with it.
  glBindVertexArray(VAO);
  // Tell OpenGL to draw with triangles, using all indices, the type of the indices, and the offset to start from
  glDrawElements(GL_TRIANGLES, (int)outIndices.size(), GL_UNSIGNED_INT, 0);
  // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
  glBindVertexArray(0);
}

void Ghost::resizeObject(){
  // get min and max
  float minX = this->vertices[0].x;
  float maxX = this->vertices[0].x;
  float minY = this->vertices[0].y;
  float maxY = this->vertices[0].y;
  float minZ = this->vertices[0].z;
  float maxZ = this->vertices[0].z;
  for (auto vertex : this->vertices) {
    if (minX > vertex.x)
      minX = vertex.x;
    if (minY > vertex.y)
      minY = vertex.y;
    if (minZ > vertex.z)
      minZ = vertex.z;
    if (maxX < vertex.x)
      maxX = vertex.x;
    if (maxY < vertex.y)
      maxY = vertex.y;
    if (maxZ < vertex.z)
      maxZ = vertex.z;
  }
  
  // calculate center
  float centerX = (minX + maxX) / 2;
  float centerY = (minY + maxY) / 2;
  float centerZ = (minZ + maxZ) / 2;
  
  // center vertices
  for (int i = 0; i < this->vertices.size(); i++) {
    vertices[i] -= glm::vec3(centerX, centerY, centerZ);
  }
  
  minX -= centerX;
  maxX -= centerX;
  minY -= centerY;
  maxY -= centerY;
  minZ -= centerZ;
  maxZ -= centerZ;
  
  float scale = 1;
  float expectedSize = 50;
  if ((maxY - minY) >= (maxX - minX) && (maxY - minY) >= (maxZ - minZ)) {
    scale = expectedSize / (maxY - minY);
  } else if ((maxZ - minZ) >= (maxX - minX)) {
    scale = expectedSize / (maxZ - minZ);
  } else {
    scale = expectedSize / (maxX - minX);
  }
  
  for (int i = 0; i < this->vertices.size(); i++) {
    vertices[i] *= scale;
  }
}


void Ghost::update(){
  if(rand() % 1000 < 5){
    changeDir();
  }
  if(!island -> inMap(coord + glm::vec2(5.0f) * dir)){
    changeDir();
  }
  else{
    coord += glm::vec2(5.0f) * dir;
    location = glm::vec3(coord.x, island->getY(coord) + 50, coord.y);
  }
}

void Ghost::changeDir(){
  float dirX = (float)rand()/(float)(RAND_MAX) * 2 - 1;
  float dirY = 1 - pow(dirX, 2);
  if(rand() % 2 > 0){
    dirY = -1 * dirY;
  }
  dir = glm::vec2(dirX, dirY);
}

bool Ghost::getHit(glm::vec3 position, glm::vec3 dir){
  if (hp <= 0) {
    return false;
  }
  glm::vec3 toGhost = (location - position);
  float thetaCosine = asin(50 / glm::length(toGhost));
  float theta2Cosine = acos(glm::dot(dir, glm::normalize(toGhost)));
  return thetaCosine > theta2Cosine;
}
