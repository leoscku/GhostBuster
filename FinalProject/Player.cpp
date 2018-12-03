//
//  Player.cpp
//  FinalProject
//
//  Created by Leo Ku on 11/30/18.
//  Copyright © 2018 Cain. All rights reserved.
//

#include "Player.h"

Player::Player(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), forwardDir(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
  
  gun = new OBJObject("M4A1.obj", "web.PPM");
  
  //gun -> scale(0.01f);
  gun -> setPosition(glm::vec3(11.0f, -17.0f, -38.0f));
  //gun -> rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(5.0f));
  gun -> setFront(front, up);
  
  
  this -> position = position;
  this -> worldUp = up;
  this -> yaw = yaw;
  this -> pitch = pitch;
  updateCameraVectors();
}

Player::Player(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), forwardDir(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
  
  gun = new OBJObject("M4A1.obj", "web.PPM");
  
  this -> position = glm::vec3(posX, posY, posZ);
  this -> worldUp = glm::vec3(upX, upY, upZ);
  this -> yaw = yaw;
  this -> pitch = pitch;
  updateCameraVectors();
}

glm::mat4 Player::getViewMatrix()   {
  return glm::lookAt(position, position + front, up);
}

void Player::processKeyboard (Camera_Movement direction, float deltaTime) {
  float velocity = movementSpeed * deltaTime;
  if (direction == FORWARD) {
    position += forwardDir * velocity;
    gun -> setPosition((forwardDir * velocity));
  }
  if (direction == BACKWARD) {
    position -= forwardDir * velocity;
    gun -> setPosition(-forwardDir * velocity);
  }
  if (direction == LEFT) {
    position -= rightDir * velocity;
    gun -> setPosition(-rightDir * velocity);
  }
  if (direction == RIGHT) {
    position += rightDir * velocity;
    gun -> setPosition(rightDir * velocity);
  }
  
}

void Player::processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
  xoffset *= mouseSensitivity;
  yoffset *= mouseSensitivity;
  
  yaw   += xoffset;
  pitch += yoffset;
  
  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch)
  {
    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;
  }
  
  // Update Front, Right and Up Vectors using the updated Euler angles
  updateCameraVectors();
}

void Player::processMouseScroll(float yoffset){
  if (zoom >= 1.0f && zoom <= 45.0f)
    zoom -= yoffset;
  if (zoom <= 1.0f)
    zoom = 1.0f;
  if (zoom >= 45.0f)
    zoom = 45.0f;
}

void Player::updateCameraVectors() {
  // Calculate the new Front vector
  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  
  glm::vec3 direction = glm::normalize(front) - this->front;
  float velocity = glm::length(direction);
  glm::vec3 rotAxis = glm::cross(this->front, glm::normalize(front));
  float rotAngle = velocity;
  
  this -> front = glm::normalize(front);
  forwardDir = glm::vec3(this -> front.x, 0.0f, this -> front.z);
  
  // Also re-calculate the Right and Up vector
  right = glm::normalize(glm::cross(this -> front, worldUp));
  rightDir = glm::normalize(glm::cross(forwardDir, worldUp));
  
  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  up = glm::normalize(glm::cross(right, this-> front));
  
  //gun -> rotate(rotAxis, rotAngle);
  gun -> lookAt(this -> front, up);
  //gun -> rotateEuler(yaw, pitch);
}

void Player::draw(GLuint shaderProgram) {
  gun -> draw(shaderProgram, getViewMatrix());
}
