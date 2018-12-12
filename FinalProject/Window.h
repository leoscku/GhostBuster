#ifndef _WINDOW_H_
#define _WINDOW_H_


#include <iostream>

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "Cube.h"
#include "shader.h"
#include "OBJObject.h"
#include "fmod.hpp"
#include "fmod.h"

#include "Island.h"
#include "Player.h"
#include "Bezier.h"
#include "Skybox.h"
#include "MatrixTransform.h"
#include "ParticleGenerator.h"
#include "Ghost.h"

class Window
{
public:
    static int width;
	static int height;
	static glm::mat4 P; // P for projection
	static glm::mat4 V; // V for view
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  static void mouse_callback(GLFWwindow* window, int button, int actions, int mods);
  static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
  
  static OBJObject* obj;
  
  static glm::vec3 calTrackBallVec(double x, double y);
  static glm::vec3 calRotateVec(glm::vec3 prev, glm::vec3 curr);
  static void processInput(GLFWwindow *window);
    
  static glm::vec3 cam_pos;
  static Skybox* skybox;
};

#endif
