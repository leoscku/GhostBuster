//
//  MatrixTransform.h
//  hw3
//
//  Created by Cain on 10/30/18.
//  Copyright © 2018 Cain. All rights reserved.
//

#ifndef MatrixTransform_h
#define MatrixTransform_h

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <list>

#include "Node.h"
#include "Geometry.h"

class MatrixTransform: public Node
{
public:
    MatrixTransform(glm::mat4 transform) : transform(transform){};
    ~MatrixTransform(); 
    glm::mat4 transform;
    void draw(int program, glm::mat4 M);
    void addChild(Node* node);
    void update();
    glm::mat4 getTransform();
    void setTransform(glm::mat4 trans); 
private:
    std::list<Node*> childList; 
};

#endif /* MatrixTransform_h */
