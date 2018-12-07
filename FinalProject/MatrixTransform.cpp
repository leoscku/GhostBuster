//
//  MatrixTransform.c
//  hw3
//
//  Created by Cain on 10/30/18.
//  Copyright © 2018 Cain. All rights reserved.
//

#include "MatrixTransform.h"

MatrixTransform::~MatrixTransform() {
    for (auto trans : childList) {
        delete trans;
    }
}

void MatrixTransform::draw(int program, glm::mat4 M) {
    for (auto trans : childList) {
        trans->draw(program, M * transform);
    }
}

void MatrixTransform::update() {
    
}

void MatrixTransform::addChild(Node* node) {
    childList.push_back(node);
}

glm::mat4 MatrixTransform::getTransform() {
    return transform;
}

void MatrixTransform::setTransform(glm::mat4 trans) {
    transform = trans;
    
}
