#include "Bezier.h"
#include <math.h>

Bezier::Bezier()
{
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_LINE_SMOOTH);
    toWorld = glm::mat4(1.0f);
    
    calculateVerticesIndices();

    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO2);
    
    glGenBuffers(1, &EBO);
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat) * 3, &vertices[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat) * 3, &normals[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Bezier::~Bezier() {
    // Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a
    // large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Bezier::draw(int program, glm::mat4 M)
{
    glUseProgram(program);
    // Calculate the combination of the model and view (camera inverse) matrices
    glm::mat4 modelview = Window::V * M * toWorld;
    glm::mat4 model = M * toWorld;
    uProjection = glGetUniformLocation(program, "projection");
    uModelview = glGetUniformLocation(program, "modelview");
    
    // Now send these values to the shader program
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
    
    uModel = glGetUniformLocation(program, "model");
    uModelFix = glGetUniformLocation(program, "modelFix");
    uCameraPos = glGetUniformLocation(program, "cameraPos");
    
    // Now send these values to the shader program
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
    glUniformMatrix4fv(uModel, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(uModelFix, 1, GL_FALSE, &glm::transpose(glm::inverse(model))[0][0]);
    glUniform3fv(uCameraPos, 1, &Window::cam_pos[0]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Window::skybox->skyTextureID);
    
    // Now draw the object. We simply need to bind the VAO associated with it.
    glBindVertexArray(VAO);
    
    // Tell OpenGL to draw with triangles, using all indices, the type of the indices, and the offset to start from
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    
    // glBindVertexArray(VAO2);
    // Tell OpenGL to draw with triangles, using all indices, the type of the indices, and the offset to start from
    // glDrawArrays(GL_POINTS, 0, points.size());
    

//    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
    
}

void Bezier::update()
{
    GLfloat speed = 0.002f;
    if (dir_up) {
        for (unsigned int i = 0; i < 4; i++) {
            points[i*4+1].y += speed;
            points[i*4+2].y -= speed;
        }
    } else {
        for (unsigned int i = 0; i < 4; i++) {
            points[i*4+1].y -= speed;
            points[i*4+2].y += speed;
        }
    }
    
    if (points[1].y <= -0.05f) {
        dir_up = true;
    }
    
    if (points[1].y >= 0.05f) {
        dir_up = false;
    }
    calculateVerticesIndices();
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat) * 3, &vertices[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat) * 3, &normals[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_DYNAMIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Bezier::calculateVerticesIndices() {
    normals.clear();
    vertices.clear();
    indices.clear();
    
    int segmentNum = 20;
    std::vector<std::vector<glm::vec3>> lines; // all 4 rows
    lines.push_back(std::vector<glm::vec3>());
    lines.push_back(std::vector<glm::vec3>());
    lines.push_back(std::vector<glm::vec3>());
    lines.push_back(std::vector<glm::vec3>());
    
    // calculate bezier curve for the 4 rows
    for (unsigned int i = 0; i < 4; i++) {
        glm::vec3 p0 = points[i*4+0];
        glm::vec3 p1 = points[i*4+1];
        glm::vec3 p2 = points[i*4+2];
        glm::vec3 p3 = points[i*4+3];
        
        glm::vec3 a = -p0 + 3.0f*p1 - 3.0f*p2 + p3;
        glm::vec3 b = 3.0f*p0 - 6.0f*p1 + 3.0f*p2;
        glm::vec3 c = -3.0f*p0 + 3.0f*p1;
        glm::vec3 d = p0;
        
        for (unsigned int j = 0; j < segmentNum; j++) {
            float t = (float)j / (float)(segmentNum-1);
            glm::vec3 x = a*(float)pow(t, 3) + b*(float)pow(t, 2) + c*t + d;
            lines[i].push_back(x);
        }
    }
    
    std::vector<std::vector<glm::vec3>> lines2; // all 4 columns
    lines2.push_back(std::vector<glm::vec3>());
    lines2.push_back(std::vector<glm::vec3>());
    lines2.push_back(std::vector<glm::vec3>());
    lines2.push_back(std::vector<glm::vec3>());
    
    // calculat3e bezier curve for the 4 columns
    for (unsigned int i = 0; i < 4; i++) {
        glm::vec3 p0 = points[i];
        glm::vec3 p1 = points[i+4];
        glm::vec3 p2 = points[i+8];
        glm::vec3 p3 = points[i+12];
        
        glm::vec3 a = -p0 + 3.0f*p1 - 3.0f*p2 + p3;
        glm::vec3 b = 3.0f*p0 - 6.0f*p1 + 3.0f*p2;
        glm::vec3 c = -3.0f*p0 + 3.0f*p1;
        glm::vec3 d = p0;
        
        for (unsigned int j = 0; j < segmentNum; j++) {
            float t = (float)j / (float)(segmentNum-1);
            glm::vec3 x = a*(float)pow(t, 3) + b*(float)pow(t, 2) + c*t + d;
            lines2[i].push_back(x);
        }
    }
    
    for (unsigned int i = 0; i < segmentNum; i++) {
        float u = (float)i / (float)(segmentNum-1);
        glm::vec3 p00 = lines[0][i];
        glm::vec3 p01 = lines[1][i];
        glm::vec3 p02 = lines[2][i];
        glm::vec3 p03 = lines[3][i];
        
        glm::vec3 a0 = -p00 + 3.0f*p01 - 3.0f*p02 + p03;
        glm::vec3 b0 = 3.0f*p00 - 6.0f*p01 + 3.0f*p02;
        glm::vec3 c0 = -3.0f*p00 + 3.0f*p01;
        glm::vec3 d0 = p00;
        
        for (unsigned int j = 0; j < segmentNum; j++) {
            float v = (float)j / (float)(segmentNum-1);
            glm::vec3 p10 = lines2[0][j];
            glm::vec3 p11 = lines2[1][j];
            glm::vec3 p12 = lines2[2][j];
            glm::vec3 p13 = lines2[3][j];
            
            glm::vec3 a1 = -p10 + 3.0f*p11 - 3.0f*p12 + p13;
            glm::vec3 b1 = 3.0f*p10 - 6.0f*p11 + 3.0f*p12;
            glm::vec3 c1 = -3.0f*p10 + 3.0f*p11;
            glm::vec3 d1 = p10;
            
            glm::vec3 x = a0*(float)pow(v, 3) + b0*(float)pow(v, 2) + c0*v + d0;
            vertices.push_back(x);
            
            glm::vec3 dx = 3.0f*a0*v*v + 2.0f*b0*v + c0;
            glm::vec3 dy = 3.0f*a1*u*u + 2.0f*b1*u + c1;
            glm::vec3 n = glm::normalize(glm::cross(dy, dx));
            normals.push_back(n);
        }
    }
    
    /*for (unsigned int i = 0; i < segmentNum; i++) {
        float u = (float)i / (float)(segmentNum-1);
        glm::vec3 p00 = lines[0][i];
        glm::vec3 p01 = lines[1][i];
        glm::vec3 p02 = lines[2][i];
        glm::vec3 p03 = lines[3][i];
        
        glm::vec3 a0 = -p00 + 3.0f*p01 - 3.0f*p02 + p03;
        glm::vec3 b0 = 3.0f*p00 - 6.0f*p01 + 3.0f*p02;
        glm::vec3 c0 = -3.0f*p00 + 3.0f*p01;
        // glm::vec3 d0 = p00;
        glm::vec3 dx = 3.0f*a0*u*u + 2.0f*b0*u + c0;
        for (unsigned int j = 0; j < segmentNum; j++) {
            float v = (float)j / (float)(segmentNum-1);
            
            glm::vec3 p10 = lines2[0][j];
            glm::vec3 p11 = lines2[1][j];
            glm::vec3 p12 = lines2[2][j];
            glm::vec3 p13 = lines2[3][j];
            
            glm::vec3 a1 = -p10 + 3.0f*p11 - 3.0f*p12 + p13;
            glm::vec3 b1 = 3.0f*p10 - 6.0f*p11 + 3.0f*p12;
            glm::vec3 c1 = -3.0f*p10 + 3.0f*p11;
            // glm::vec3 d1 = p10;
            
            // glm::vec3 dy = 3.0f*a1*(float)pow(v, 2) + 2.0f*b1*v + c1;
            glm::vec3 dy = 3.0f*a1*v*v + 2.0f*b1*v + c1;
            glm::vec3 n = glm::normalize(glm::cross(dx, dy));
            normals.push_back(n);
        }
    }*/
    
    for (unsigned int i = 0; i < segmentNum-1; i++) {
        for (unsigned int j = 0; j < segmentNum-1; j++) {
            indices.push_back(j*segmentNum+i);
            indices.push_back(j*segmentNum+i+1);
            indices.push_back((j+1)*segmentNum+i);
            indices.push_back((j+1)*segmentNum+i);
            indices.push_back(j*segmentNum+i+1);
            indices.push_back((j+1)*segmentNum+i+1);
        }
    }
    
}

void Bezier::scale(float coefficient) {
    toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(coefficient)) * toWorld;
}
