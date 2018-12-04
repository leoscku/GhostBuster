//
//  island.cpp
//  FinalProject
//
//  Created by Muyuan Chi on 2018/11/27.
//  Copyright © 2018年 Cain. All rights reserved.
//

#include "Island.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


Island::Island(){
    srand(time(NULL));
    for (int x = 0; x < mapSize; x++){
        for (int z = 0;z < mapSize; z++){
            mapArray[x][z] = 0;
        }
    }
    
    texID = loadTexture();
    
    generateRandomMap(glm::vec2(0.0f));
    
    bufferData();
    /*
    for (int x = 0; x < mapSize; x++){
        for (int z = 0;z < mapSize; z++){
            std::cout << mapArray[x][z] << "\t";
        }
        std:: cout << std::endl;
    }*/
}

// generate the map
void Island::generateRandomMap(glm::vec2 centerPoint){
    // initialize corner
    mapArray[0][0] = 0.0f;
    mapArray[0][mapSize - 1] = 0.0f;
    mapArray[mapSize - 1][0] = 0.0f;
    mapArray[mapSize - 1][mapSize - 1] = 0.0f;

    // recursive call
    mapSquare(mapSize);
}

// generate a point
void Island::mapSquare(int size){
    int half = size / 2;
    if (half < 1){
        return;
    }
    
    // square step part
    for (int z = half; z < mapSize; z += size){
        for (int x = half; x < mapSize; x += size){
            squareStep(x % mapSize, z % mapSize, half);
        }
    }
    
    int column = 0;
    
    // diamond step part
    for (int x = 0; x < mapSize; x+= half){
        column++;
        
        
        // half*2 will work, size will not
        if (column % 2 == 1){
            for (int z = half; z < mapSize; z += half * 2){
                diamondStep(x % mapSize, z % mapSize, half);
            }
        }
        
        else{
            for (int z = 0; z < mapSize; z += half * 2){
                diamondStep(x % mapSize, z % mapSize, half);
            }
        }
    }
    
    mapSquare(size / 2);
}

void Island::squareStep(int x, int z, int length){
    int size = length;
    int pointNum = 0;
    float sum = 0.0f;
    
    // get rid off index out off bound
    if (x - length >= 0 && z - length >= 0){
        pointNum ++;
        sum += mapArray[x - length][z - length];
    }
    
    if (x + length < mapSize && z - length >= 0){
        pointNum ++;
        sum += mapArray[x + length][z - length];
    }
    
    if (x - length >= 0 && z + length < mapSize){
        pointNum ++;
        sum += mapArray[x - length][z + length];
    }
    
    if (x + length < mapSize && z + length < mapSize){
        pointNum ++;
        sum += mapArray[x + length][z + length];
    }
    
    sum /= pointNum;
    
    float delta = ((float)rand()/(float)(RAND_MAX)) * float(size) - float(size) * 0.5f;
    sum += delta / 1.5 ;

    mapArray[x][z] = sum;
}

void Island::diamondStep(int x, int z, int length){
    int size = length;

    int pointNum = 0;
    float sum = 0.0f;
    
    if (x - length >= 0){
        pointNum ++;
        sum += mapArray[x - length][z];
    }
    
    if (x + length < mapSize){
        pointNum ++;
        sum += mapArray[x + length][z];
    }
    
    if (z + length < mapSize){
        pointNum ++;
        sum += mapArray[x][z + length];
    }
    
    if (z - length >= 0){
        pointNum ++;
        sum += mapArray[x][z - length];
    }
    
    sum /= pointNum;
    
    float delta = ((float)rand()/(float)(RAND_MAX)) * float(size) - float(size) * 0.5f;

    sum += delta / 1.5 ;
    
    mapArray[x][z] = sum;
}

void Island::bufferData(){
    int c = 0;
    
    for (int x = 0; x < mapSize ; x++){
        for (int z = 0;z < mapSize ; z++){
            coordData.push_back(glm::vec3(x * squareSize - mapSize / 2 * squareSize, mapArray[x][z], z * squareSize - mapSize / 2 * squareSize));
            texture.push_back(glm::vec2(float(x) / (mapSize - 1), float(z) / (mapSize - 1)));
            normals[x][z] = glm::vec3(0);
        }
    }
    for (int x = 0; x < mapSize - 1; x++){
        for (int z = 0;z < mapSize - 1; z++){
            indices.push_back(x * mapSize + z);
            indices.push_back(x * mapSize + z + 1);
            indices.push_back((x + 1) * mapSize + z + 1);
            indices.push_back(x * mapSize + z);
            indices.push_back((x + 1) * mapSize + z );
            indices.push_back((x + 1) * mapSize + z + 1);
            
            glm::vec3 first = glm::vec3(x * squareSize - mapSize / 2 * squareSize, mapArray[x][z + 1], (z + 1) * squareSize - mapSize / 2 * squareSize) - glm::vec3(x * squareSize - mapSize / 2 * squareSize, mapArray[x][z], z * squareSize - mapSize / 2 * squareSize);
            
            glm::vec3 second = glm::vec3((x + 1) * squareSize - mapSize / 2 * squareSize, mapArray[x + 1][z], z * squareSize - mapSize / 2 * squareSize) - glm::vec3(x * squareSize - mapSize / 2 * squareSize, mapArray[x][z], z * squareSize - mapSize / 2 * squareSize);
            
            glm::vec3 res = glm::normalize(glm::cross(first, second));
            
            normals[x][z] += res;
            normals[x + 1][z] += res;
            normals[x][z + 1] += res;
            normals[x + 1][z + 1] += res;

        }
    }

    for (int x = 0; x < mapSize ; x++){
        for (int z = 0;z < mapSize ; z++){
            normals[x][z] = glm::normalize(normals[x][z]);
        }
    }
    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &CBO);
    
    // Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
    // Consider the VAO as a container for all your buffers.
    glBindVertexArray(VAO);
    
    // Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
    // you want to draw, such as vertices, normals, colors, etc.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferData(GL_ARRAY_BUFFER, coordData.size() * sizeof(GLfloat) * 3, &coordData[0], GL_STATIC_DRAW);
    // Enable the usage of layout location 0 (check the vertex shader to see what this is)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
                          3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
                          GL_FLOAT, // What type these components are
                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                          3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                          (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);
    
    glBindBuffer(GL_ARRAY_BUFFER, CBO);
    
    glBufferData(GL_ARRAY_BUFFER, texture.size() * 2 * sizeof(GLfloat), &texture[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
                          2, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
                          GL_FLOAT, // What type these components are
                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                          2 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                          (GLvoid*) 0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    
    // Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
    // you want to draw, such as vertices, normals, colors, etc.
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), &normals, GL_STATIC_DRAW);
    // Enable the usage of layout location 0 (check the vertex shader to see what this is)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
                          3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
                          GL_FLOAT, // What type these components are
                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                          3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                          (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.

    // Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO now so we don't accidentally tamper with it.
    // NOTE: You must NEVER unbind the element array buffer associated with a VAO!
    glBindVertexArray(0);
    
    
}

void Island::draw(GLuint shaderProgram, glm::mat4 view)
{
    glUseProgram(shaderProgram);

    // Calculate the combination of the model and view (camera inverse) matrices
    glm::mat4 modelview = view;
    // We need to calculate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
    // Consequently, we need to forward the projection, view, and model matrices to the shader programs
    // Get the location of the uniform variables "projection" and "modelview"
    uProjection = glGetUniformLocation(shaderProgram, "projection");
    uModelview = glGetUniformLocation(shaderProgram, "modelview");
    
    // Now send these values to the shader program
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
    
    int texIdPos = glGetUniformLocation(shaderProgram, "tex");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glUniform1i(texIdPos, 0);
    
    // Now draw the object. We simply need to bind the VAO associated with it.
    glBindVertexArray(VAO);
    
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
}

void Island::reGenerateData(){
    srand(time(NULL));
    mapSquare(mapSize);
    
    coordData.clear();
    for (int x = 0; x < mapSize ; x++){
        for (int z = 0;z < mapSize ; z++){
            coordData.push_back(glm::vec3(x * squareSize - mapSize / 2 * squareSize, mapArray[x][z], z * squareSize - mapSize / 2 * squareSize));
        }
    }
    
    
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, coordData.size() * sizeof(GLfloat) * 3, &coordData[0]);
    
    for (int x = 0; x < mapSize ; x++){
        for (int z = 0;z < mapSize ; z++){
            normals[x][z] = glm::vec3(0);
        }
    }
    
    for (int x = 0; x < mapSize - 1; x++){
        for (int z = 0;z < mapSize - 1; z++){
            glm::vec3 first = glm::vec3(x * squareSize - mapSize / 2 * squareSize, mapArray[x][z + 1], (z + 1) * squareSize - mapSize / 2 * squareSize) - glm::vec3(x * squareSize - mapSize / 2 * squareSize, mapArray[x][z], z * squareSize - mapSize / 2 * squareSize);
            
            glm::vec3 second = glm::vec3((x + 1) * squareSize - mapSize / 2 * squareSize, mapArray[x + 1][z], z * squareSize - mapSize / 2 * squareSize) - glm::vec3(x * squareSize - mapSize / 2 * squareSize, mapArray[x][z], z * squareSize - mapSize / 2 * squareSize);
            
            normals[x][z] += glm::normalize(glm::cross(first, second));
            normals[x + 1][z] += glm::normalize(glm::cross(first, second));
            normals[x][z + 1] += glm::normalize(glm::cross(first, second));
            normals[x + 1][z + 1] += glm::normalize(glm::cross(first, second));
            
        }
    }
    
    for (int x = 0; x < mapSize ; x++){
        for (int z = 0;z < mapSize ; z++){
            normals[x][z] = glm::normalize(normals[x][z]);
        }
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(normals), &normals[0]);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

int Island::loadTexture(){
    
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    int width, height, nrChannels;
    
    unsigned char *data = stbi_load("terrainTex.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D,
                        0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
                        );
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Cubemap texture failed to load" << std::endl;
        stbi_image_free(data);
    }
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    return textureID;
}

float Island::getY(glm::vec2 coord){
    // using x, z since y is "up"
    float x = coord.x;
    float z = coord.y;
    
    // perform linear interpolation
    int xPos = int((x + mapSize / 2 * squareSize) / squareSize);
    int zPos = int((z + mapSize / 2 * squareSize) / squareSize);
    std::cout << xPos << " " << zPos << std::endl;

    float xCoef = fmod(x + mapSize / 2 * squareSize, squareSize);
    float zCoef = fmod(z + mapSize / 2 * squareSize, squareSize);

    float fp = (mapArray[xPos + 1][zPos] - mapArray[xPos][zPos]) * xCoef + mapArray[xPos][zPos];
    float sp = (mapArray[xPos + 1][zPos + 1] - mapArray[xPos][zPos + 1]) * xCoef + mapArray[xPos][zPos + 1];

    float res = zCoef * (sp - fp) + fp;
    return res;
}

bool Island::inMap(glm::vec2 coord){
    return coord.x < mapSize / 2 * squareSize && coord.x > -1 * mapSize / 2 * squareSize &&
        coord.y < mapSize / 2 * squareSize && coord.y > -1 * mapSize / 2 * squareSize;
}
