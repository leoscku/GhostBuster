#include "ParticleGenerator.h"

// Muzzle-flash sparks are short-lived: a quick pop, not a lingering cloud.
static const float LIFETIME = 0.12f;

ParticleGenerator::ParticleGenerator(int amount, Player* p)
: amount(amount), player(p)
{
  this->init();
}

void ParticleGenerator::update(float dt, glm::vec3 position, int newParticles, glm::vec3 offset)
{
  // Add new particles
  for (int i = 0; i < newParticles; ++i)
  {
    int unusedParticle = this->firstUnusedParticle();
    this->respawnParticle(this->particles[unusedParticle], position, offset);
  }
  // Update all particles
  for (int i = 0; i < this->amount; ++i)
  {
    Particle &p = this->particles[i];
    p.life -= dt; // reduce life
    if (p.life > 0.0f)
    {  // particle is alive, thus update
      float lifeFrac = p.life / LIFETIME;
      p.position += p.velocity * dt;   // fly OUTWARD from the muzzle (was -=, drifted backward)
      p.color.a = lifeFrac;            // fade out over life (now that additive blending is on)
      p.color.g = 0.85f * lifeFrac;    // cool from hot white-yellow...
      p.color.b = 0.45f * lifeFrac;    // ...through orange to red as it dies
    }
  }
}

// Render all particles
void ParticleGenerator::draw(GLuint shaderProgram, glm::mat4 view)
{
  // Additive blending makes overlapping sparks glow; keep depth TEST (so the
  // flash is occluded by terrain/gun) but disable depth WRITE so the sparks
  // blend with each other instead of fighting the depth buffer.
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  glDepthMask(GL_FALSE);

  // Uniform locations and the view matrix are constant across the batch — fetch
  // them once instead of 4x per particle every frame.
  glm::mat4 modelview = view;
  GLint uProj   = glGetUniformLocation(shaderProgram, "projection");
  GLint uMV     = glGetUniformLocation(shaderProgram, "modelview");
  GLint uOffset = glGetUniformLocation(shaderProgram, "offset");
  GLint uColor  = glGetUniformLocation(shaderProgram, "color");
  GLint uScale  = glGetUniformLocation(shaderProgram, "pscale");
  glUniformMatrix4fv(uProj, 1, GL_FALSE, &Window::P[0][0]);
  glUniformMatrix4fv(uMV, 1, GL_FALSE, &modelview[0][0]);

  glBindVertexArray(VAO);
  for (Particle &particle : this->particles)
  {
    if (particle.life > 0.0f)
    {
      float lifeFrac = particle.life / LIFETIME;
      glUniform3f(uOffset, particle.position.x, particle.position.y, particle.position.z);
      glUniform4f(uColor, particle.color.r, particle.color.g, particle.color.b, particle.color.a);
      glUniform1f(uScale, 6.0f + 16.0f * lifeFrac);   // bigger when fresh, shrinks as it dies
      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }
  }
  glBindVertexArray(0);

  // Restore default state for the opaque passes that follow.
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
}

void ParticleGenerator::init()
{
  translationM = glm::mat4(1.0f);
  displacementM = glm::translate(glm::mat4(1.0), glm::vec3(11.0f, -17.0f, -38.0f));
  
  // Set up mesh and attribute properties
  GLfloat particle_quad[8][3] = {
    // "Front" vertices
    {-0.05, -0.05,  0.05}, {0.05, -0.05,  0.05}, {0.05,  0.05,  0.05}, {-0.05,  0.05,  0.05},
    // "Back" vertices
    {-0.05, -0.05, -0.05}, {0.05, -0.05, -0.05}, {0.05,  0.05, -0.05}, {-0.05,  0.05, -0.05}
  };
  
  GLuint indices[6][6] = {
    // Front face
    {0, 1, 2, 2, 3, 0},
    // Top face
    {1, 5, 6, 6, 2, 1},
    // Back face
    {7, 6, 5, 5, 4, 7},
    // Bottom face
    {4, 0, 3, 3, 7, 4},
    // Left face
    {4, 5, 1, 1, 0, 4},
    // Right face
    {3, 2, 6, 6, 7, 3}
  };

  
  glGenVertexArrays(1, &this->VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  
  glBindVertexArray(this->VAO);
  // Fill mesh buffer
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
  // Set mesh attributes
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
  
  // Create this->amount default particle instances
  for (int i = 0; i < this->amount; ++i)
    this->particles.push_back(Particle());
}

// Stores the index of the last particle used (for quick access to next dead particle)
int lastUsedParticle = 0;
GLuint ParticleGenerator::firstUnusedParticle()
{
  // First search from last used particle, this will usually return almost instantly
  for (int i = lastUsedParticle; i < this->amount; ++i){
    if (this->particles[i].life <= 0.0f){
      lastUsedParticle = i;
      return i;
    }
  }
  // Otherwise, do a linear search
  for (int i = 0; i < lastUsedParticle; ++i){
    if (this->particles[i].life <= 0.0f){
      lastUsedParticle = i;
      return i;
    }
  }
  // All particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
  lastUsedParticle = 0;
  return 0;
}

void ParticleGenerator::respawnParticle(Particle &particle, glm::vec3 position, glm::vec3 offset)
{
  int max = 100;
  int base = 50;
  float angle;
  
  glm::vec3 front = player -> getFront();
  
  float randomX = ((rand() % max) - base) / 10.0f;
  float randomY = ((rand() % max) - base) / 10.0f;
  float randomZ = ((rand() % max) - base) / 10.0f;
  
  
  glm::vec3 direction = glm::normalize(glm::vec3(randomX, randomY, randomZ));
  
  angle = acos(glm::dot(front, direction));
  
  // Keep directions inside a tight forward cone so sparks spray out the barrel.
  while (angle > glm::radians(28.0f)) {
    randomX = ((rand() % max) - base) / 10.0f;
    randomY = ((rand() % max) - base) / 10.0f;
    randomZ = ((rand() % max) - base) / 10.0f;

    direction = glm::normalize(glm::vec3(randomX, randomY, randomZ));

    angle = acos(glm::dot(front, direction));
  }

  // Small jitter around the muzzle so the burst isn't a single point.
  glm::vec3 jitter = glm::vec3(randomX, randomY, randomZ) * 0.06f;
  particle.position = position + jitter + offset;
  particle.color = glm::vec4(1.0f, 0.85f, 0.45f, 1.0f);  // hot white-yellow core
  particle.life = LIFETIME;
  particle.velocity = direction * 60.0f;                 // fly out fast, then die quick
}
