

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

// Represents a single particle and its state
struct Particle {
  glm::vec3 position, velocity;
  glm::vec4 color;
  float life;
  
  Particle() : position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) { }
};

class ParticleGenerator
{
public:
  // Constructor
  ParticleGenerator(int amount);
  // Update all particles
  void update(GLfloat dt, glm::vec3 position, GLuint newParticles, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));
  // Render all particles
  void Draw();
private:
  // State
  std::vector<Particle> particles;
  int amount;
  
  // Render state
  Shader shader;
  Texture2D texture;
  GLuint VAO;
  
  // Initializes buffer and vertex attributes
  void init();
  // Returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
  GLuint firstUnusedParticle();
  // Respawns particle
  void respawnParticle(Particle &particle, glm::vec3 position, glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f));
};


#endif /* ParticleGenerator_hpp */
