

#ifndef ParticleGenerator_h
#define ParticleGenerator_h

#include <stdio.h>

#ifdef __APPLE__
#include <iostream>
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include <random>

#include "Window.h"
#include "Player.h"

// Represents a single particle and its state
struct Particle {
  glm::vec3 position, velocity;
  glm::vec4 color;
  float life;
  
  Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) { }
};

class Player;

class ParticleGenerator
{
public:
  // Constructor
  ParticleGenerator(int amount, Player* p);
  // Update all particles
  void update(float dt, glm::vec3 position, int newParticles, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));
  // Render all particles
  void draw(GLuint shaderProgram, glm::mat4 view);
private:
  Player* player;
  glm::mat4 displacementM;
  // State
  std::vector<Particle> particles;
  int amount;
  
  // Render state
  GLuint VBO, VBO2, VBO3, VAO, EBO;
  GLuint uProjection, uModelview, uTex;
  
  glm::mat4 translationM;
  
  
  
  // Initializes buffer and vertex attributes
  void init();
  // Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
  GLuint firstUnusedParticle();
  // Respawns particle
  void respawnParticle(Particle &particle, glm::vec3 position, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));
};


#endif /* ParticleGenerator_hpp */
