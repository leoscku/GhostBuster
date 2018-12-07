//
//  Player.cpp
//  FinalProject
//
//  Created by Leo Ku on 11/30/18.
//  Copyright © 2018 Cain. All rights reserved.
//

#include "Player.h"

Player::Player(glm::vec3 position, Island* island, glm::vec3 up, float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), forwardDir(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
  
  gun = new OBJObject("M4A1.obj", "web.PPM");
  
  gun -> initializeVector(front, right, up);
  this -> island = island;
  
  float yValue = island -> getY(glm::vec2(position.x, position.z));
  
  std::cout << yValue << std::endl;
  
  this -> position = glm::vec3(position.x, yValue, position.z);
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
  }
  if (direction == BACKWARD) {
    position -= forwardDir * velocity;
  }
  if (direction == LEFT) {
    position -= rightDir * velocity;
  }
  if (direction == RIGHT) {
    position += rightDir * velocity;
  }
  
  float yValue = island -> getY(glm::vec2(position.x, position.z));
    
  this -> position = glm::vec3(position.x, yValue, position.z);

  gun -> lookAt(position, front, right, up);
  
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
  
  this -> front = glm::normalize(front);
  forwardDir = glm::vec3(this -> front.x, 0.0f, this -> front.z);
  
  // Also re-calculate the Right and Up vector
  right = glm::normalize(glm::cross(this -> front, worldUp));
  rightDir = glm::normalize(glm::cross(forwardDir, worldUp));
  
  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  up = glm::normalize(glm::cross(right, this-> front));
  
  gun -> lookAt(position, this -> front, right, up);
}

void Player::draw(GLuint shaderProgram) {
  gun -> draw(shaderProgram, getViewMatrix());
}

glm::vec3 Player::getPosition(){
    return position;
}
