//
//  Geometry.h
//  hw3
//
//  Created by Cain on 10/30/18.
//  Copyright © 2018 Cain. All rights reserved.
//

#ifndef Geometry_h
#define Geometry_h

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

#include "Node.h"
#include "OBJObject.h"

class Geometry: public Node
{
public:
    Geometry(const char* OBJfilepath, const char* TEXfilepath);
    ~Geometry();
    OBJObject* object;
    void draw(int program, glm::mat4 M);
    void update();
    void setModelView(glm::mat4 modelview);
    void scale(GLfloat coefficent);
    void spin(float deg, glm::vec3 axis);
    void translate(glm::vec3 dist);
    bool display = true;
private:
    glm::mat4 modelview;
    GLfloat scaleCoefficient;
};

#endif /* Geometry.h */
