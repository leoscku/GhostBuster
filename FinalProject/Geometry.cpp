//
//  Geometry.h
//  hw3
//
//  Created by Cain on 10/30/18.
//  Copyright © 2018 Cain. All rights reserved.
//

#include "Geometry.h"

Geometry::Geometry(const char* OBJfilepath, const char* TEXfilepath) {
    object = new OBJObject(OBJfilepath, TEXfilepath);
    modelview = glm::mat4(1.0f);
    scaleCoefficient = 1.0f;
}

void Geometry::draw(int program, glm::mat4 M) {
    if (display) {
        object->draw(program, M * glm::scale(glm::mat4(1.0f), glm::vec3(scaleCoefficient)) * modelview);
    }
}

void Geometry::update() {
    
}

void Geometry::setModelView(glm::mat4 modelview) {
    this->modelview = modelview;
}

void Geometry::scale(GLfloat coefficent) {
    scaleCoefficient *= coefficent; 
}


void Geometry::spin(float deg, glm::vec3 axis)
{
    modelview = modelview * glm::rotate(glm::mat4(1.0f), deg / 180.0f * glm::pi<float>(), axis);
}

void Geometry::translate(glm::vec3 dist) {
    modelview = modelview * glm::translate(glm::mat4(1.0f), dist);
}

Geometry::~Geometry() {
    delete object;
}
