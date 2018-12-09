//
//  Player.hpp
//  FinalProject
//
//  Created by Leo Ku on 11/30/18.
//  Copyright © 2018 Cain. All rights reserved.
//

#ifndef Player_h
#define Player_h

#ifdef __APPLE__
#include <iostream>
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
//#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>

#include <stdio.h>
#include "OBJObject.h"
#include "Island.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  200.0f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;

class Island;
class Player
{
public:
  // Camera Attributes
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;
  // Euler Angles
  float yaw;
  float pitch;
  // Camera options
  float movementSpeed;
  float mouseSensitivity;
  float zoom;
  // Player movement vector
  glm::vec3 forwardDir;
  glm::vec3 rightDir;
  
  //Player Gun
  OBJObject* gun;
  
  //Island Reference
  Island* island;
  
  
  // Constructor with vectors
  Player(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), Island* island = NULL, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
 
  // Constructor with scalar values
  Player(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
  
  // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
  glm::mat4 getViewMatrix();
  
  // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
  void processKeyboard(Camera_Movement direction, float deltaTime);
  
  // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
  void processMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
  
  // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
  void processMouseScroll(float yoffset);
  
  void draw(GLuint shaderProgram);
    
  glm::vec3 getPosition();
  
  glm::vec3 getMuzzlePosition();
  
  glm::vec3 getFront();
  
  glm::vec3 getRight();
  
private:
  // Calculates the front vector from the Camera's (updated) Euler Angles
  void updateCameraVectors();
};


#endif /* Player_hpp */
