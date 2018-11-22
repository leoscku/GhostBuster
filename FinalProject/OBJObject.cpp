#include "OBJObject.h"
#include "Window.h"
#include <math.h>

OBJObject::OBJObject(const char* OBJfilepath, const char* TEXfilepath)
{
	toWorld = glm::mat4(1.0f);
	parse(OBJfilepath);
    centerObject();
    loadTexture(TEXfilepath); 
    reorderData();
    
    // Create array object and buffers. Remember to delete your buffers when the object is destroyed!
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &VBO3);
    glGenBuffers(1, &EBO);
    
    // Bind the Vertex Array Object (VAO) first, then bind the associated buffers to it.
    // Consider the VAO as a container for all your buffers.
    glBindVertexArray(VAO);
    
    // Now bind a VBO to it as a GL_ARRAY_BUFFER. The GL_ARRAY_BUFFER is an array containing relevant data to what
    // you want to draw, such as vertices, normals, colors, etc.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData populates the most recently bound buffer with data starting at the 3rd argument and ending after
    // the 2nd argument number of indices. How does OpenGL know how long an index spans? Go to glVertexAttribPointer.
    glBufferData(GL_ARRAY_BUFFER, outVertices.size() * sizeof(GLfloat) * 3, &outVertices[0], GL_STATIC_DRAW);
    // Enable the usage of layout location 0 (check the vertex shader to see what this is)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,// This first parameter x should be the same as the number passed into the line "layout (location = x)" in the vertex shader. In this case, it's 0. Valid values are 0 to GL_MAX_UNIFORM_LOCATIONS.
                          3, // This second line tells us how any components there are per vertex. In this case, it's 3 (we have an x, y, and z component)
                          GL_FLOAT, // What type these components are
                          GL_FALSE, // GL_TRUE means the values should be normalized. GL_FALSE means they shouldn't
                          3 * sizeof(GLfloat), // Offset between consecutive indices. Since each of our vertices have 3 floats, they should have the size of 3 floats in between
                          (GLvoid*)0); // Offset of the first vertex's component. In our case it's 0 since we don't pad the vertices array with anything.
    
    // We've sent the vertex data over to OpenGL, but there's still something missing.
    // In what order should it draw those vertices? That's why we'll need a GL_ELEMENT_ARRAY_BUFFER for this.
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, outTexiles.size() * sizeof(GLfloat) * 2, &outTexiles[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, outNormals.size() * sizeof(GLfloat) * 3, &outNormals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, outIndices.size() * sizeof(GLuint), &outIndices[0], GL_STATIC_DRAW);
    
    // Unbind the currently bound buffer so that we don't accidentally make unwanted changes to it.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind the VAO now so we don't accidentally tamper with it.
    // NOTE: You must NEVER unbind the element array buffer associated with a VAO!
    glBindVertexArray(0);
}

OBJObject::~OBJObject() {
    // Delete previously generated buffers. Note that forgetting to do this can waste GPU memory in a
    // large project! This could crash the graphics driver due to memory leaks, or slow down application performance!
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &VBO3);
    glDeleteBuffers(1, &EBO);
}

void OBJObject::parse(const char *filepath) 
{
    FILE* fp;
    float x, y, z;
    float r, g, b;
    int c1, c2;
    float tx, ty;
    int f_v1, f_t1, f_n1, f_v2, f_t2, f_n2, f_v3, f_t3, f_n3;
    
    fp = fopen(filepath, "rb");
    if (fp==NULL) {std::cerr << "Error loading file" << std::endl; exit(-1); }
    while (1) {
        c1 = fgetc(fp);
        c2 = fgetc(fp);
        int error;

        while (c1 == '\n') {
            c1 = c2;
            c2 = fgetc(fp);
        }

        if (c1 == 'v' && c2 == ' ') {
            error = fscanf(fp, "%f %f %f %f %f %f\n", &x, &y, &z);
            if (error != EOF) {
                this->vertices.push_back(glm::vec3(x, y, z));
            }
        } else if (c1 == 'v' && c2 == 'n') {
            error = fscanf(fp, "%f %f %f\n", &x, &y, &z);
            if (error != EOF) {
                this->normals.push_back(glm::normalize(glm::vec3(x, y, z)));
            }
        } else if (c1 == 'v' && c2 == 't') {
            error = fscanf(fp, "%f %f\n", &tx, &ty);
            if (error != EOF) {
                this->texiles.push_back(glm::vec2(tx, ty));
            }
        } else if (c1 == 'f' && c2 == ' ') {
            error = fscanf(fp, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &f_v1, &f_t1, &f_n1, &f_v2, &f_t2, &f_n2, &f_v3, &f_t3, &f_n3);
            if (error != EOF) {
                this->indices.push_back(glm::vec3(f_v1, f_t1, f_n1));
                this->indices.push_back(glm::vec3(f_v2, f_t2, f_n2));
                this->indices.push_back(glm::vec3(f_v3, f_t3, f_n3));
            }
        }
        if (feof(fp)) break;
    }
    std::cout << "faces: " << this->indices.size() << std::endl;
    std::cout << "vetices: " << this->vertices.size() << std::endl;
    std::cout << "normals: " << this->normals.size() << std::endl;
    std::cout << "texiles: " << this->texiles.size() << std::endl;

    /*for (int i = 0; i < texiles.size(); i++) {
        std::cout << texiles[i].x << " " << texiles[i].y << std::endl;
    }*/
    fclose(fp);

}

void OBJObject::reorderData() {
    for (unsigned int i = 0; i < indices.size(); i++) {
        int vi = indices[i].x;
        int ti = indices[i].y;
        int ni = indices[i].z;
        
        outVertices.push_back(vertices[vi-1]);
        outTexiles.push_back(texiles[ti-1]);
        outNormals.push_back(normals[ni-1]);
        outIndices.push_back(i); 
    }
}

void OBJObject::draw(GLuint shaderProgram)
{
    // Calculate the combination of the model and view (camera inverse) matrices
    glm::mat4 modelview = Window::V * toWorld;
    // We need to calculate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
    // Consequently, we need to forward the projection, view, and model matrices to the shader programs
    // Get the location of the uniform variables "projection" and "modelview"
    uProjection = glGetUniformLocation(shaderProgram, "projection");
    uModelview = glGetUniformLocation(shaderProgram, "modelview");
    uTex = glGetUniformLocation(shaderProgram, "tex");
    
    // Now send these values to the shader program
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
    
    // glUniform1i(uTex, textureID);
    glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Now draw the object. We simply need to bind the VAO associated with it.
    glBindVertexArray(VAO);
    // Tell OpenGL to draw with triangles, using all indices, the type of the indices, and the offset to start from
    glDrawElements(GL_TRIANGLES, outIndices.size(), GL_UNSIGNED_INT, 0);
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
}

void OBJObject::draw(GLuint shaderProgram, glm::mat4 toWorld)
{
    // Calculate the combination of the model and view (camera inverse) matrices
    glm::mat4 modelview = Window::V * toWorld;
    // We need to calculate this because modern OpenGL does not keep track of any matrix other than the viewport (D)
    // Consequently, we need to forward the projection, view, and model matrices to the shader programs
    // Get the location of the uniform variables "projection" and "modelview"
    uProjection = glGetUniformLocation(shaderProgram, "projection");
    uModelview = glGetUniformLocation(shaderProgram, "modelview");
    uTex = glGetUniformLocation(shaderProgram, "tex");
    
    // Now send these values to the shader program
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, &Window::P[0][0]);
    glUniformMatrix4fv(uModelview, 1, GL_FALSE, &modelview[0][0]);
    
    // glUniform1i(uTex, textureID);
    glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Now draw the object. We simply need to bind the VAO associated with it.
    glBindVertexArray(VAO);
    // Tell OpenGL to draw with triangles, using all indices, the type of the indices, and the offset to start from
    glDrawElements(GL_TRIANGLES, outIndices.size(), GL_UNSIGNED_INT, 0);
    // Unbind the VAO when we're done so we don't accidentally draw extra stuff or tamper with its bound buffers
    glBindVertexArray(0);
}

void OBJObject::update()
{
}

void OBJObject::centerObject() {
    // get min and max
    float minX = this->vertices[0].x;
    float maxX = this->vertices[0].x;
    float minY = this->vertices[0].y;
    float maxY = this->vertices[0].y;
    float minZ = this->vertices[0].z;
    float maxZ = this->vertices[0].z;
    for (auto vertex : this->vertices) {
        if (minX > vertex.x)
            minX = vertex.x;
        if (minY > vertex.y)
            minY = vertex.y;
        if (minZ > vertex.z)
            minZ = vertex.z;
        if (maxX < vertex.x)
            maxX = vertex.x;
        if (maxY < vertex.y)
            maxY = vertex.y;
        if (maxZ < vertex.z)
            maxZ = vertex.z;
    }
    
    // calculate center
    float centerX = (minX + maxX) / 2;
    float centerY = (minY + maxY) / 2;
    float centerZ = (minZ + maxZ) / 2;
    
    // center vertices
    for (int i = 0; i < this->vertices.size(); i++) {
        vertices[i] -= glm::vec3(centerX, centerY, centerZ);
    }
    
    minX -= centerX;
    maxX -= centerX;
    minY -= centerY;
    maxY -= centerY;
    minZ -= centerZ;
    maxZ -= centerZ;
    
    float scale = 0;
    float expectedSize = 20;
    if ((maxY - minY) >= (maxX - minX) && (maxY - minY) >= (maxZ - minZ)) {
        scale = expectedSize / (maxY - minY);
    } else if ((maxZ - minZ) >= (maxX - minX)) {
        scale = expectedSize / (maxZ - minZ);
    } else {
        scale = expectedSize / (maxX - minX);
    }
    
    for (int i = 0; i < this->vertices.size(); i++) {
        vertices[i] *= scale;
    }
}

void OBJObject::scale(float coefficient) {
    toWorld = glm::scale(glm::mat4(1.0f), glm::vec3(coefficient)) * toWorld;
}

void OBJObject::setPosition(glm::vec3 pos) {
    toWorld = glm::translate(toWorld, glm::vec3(pos));
}

/** Load a ppm file from disk.
 @input filename The location of the PPM file.  If the file is not found, an error message
 will be printed and this function will return 0
 @input width This will be modified to contain the width of the loaded image, or 0 if file not found
 @input height This will be modified to contain the height of the loaded image, or 0 if file not found
 @return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
 **/
unsigned char* OBJObject::loadPPM(const char* filename, int& width, int& height)
{
    const int BUFSIZE = 128;
    FILE* fp;
    unsigned int read;
    unsigned char* rawData;
    char buf[3][BUFSIZE];
    char* retval_fgets;
    size_t retval_sscanf;
    
    if ( (fp=fopen(filename, "rb")) == NULL)
    {
        std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
        width = 0;
        height = 0;
        return NULL;
    }
    
    // Read magic number:
    retval_fgets = fgets(buf[0], BUFSIZE, fp);
    
    // Read width and height:
    do
    {
        retval_fgets=fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');
    retval_sscanf=sscanf(buf[0], "%s %s", buf[1], buf[2]);
    width  = atoi(buf[1]);
    height = atoi(buf[2]);
    
    // Read maxval:
    do
    {
        retval_fgets=fgets(buf[0], BUFSIZE, fp);
    } while (buf[0][0] == '#');
    
    // Read image data:
    rawData = new unsigned char[width * height * 3];
    read = fread(rawData, width * height * 3, 1, fp);
    fclose(fp);
    if (read != 1)
    {
        std::cerr << "error parsing ppm file, incomplete data" << std::endl;
        delete[] rawData;
        width = 0;
        height = 0;
        return NULL;
    }
    
    return rawData;
}

// load image file into texture object
void OBJObject::loadTexture(const char* filepath)
{
    int twidth, theight;   // texture width/height [pixels]
    unsigned char* tdata;  // texture pixel data
    
    // Load image file
    tdata = loadPPM(filepath, twidth, theight);
    if (tdata==NULL) return;
    // Create ID for texture
    glGenTextures(1, &textureID);
    
    // Set this texture to be the one we are working with
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    // Generate the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, twidth, theight, 0, GL_RGB, GL_UNSIGNED_BYTE, tdata);
    
    // Set bi-linear filtering for both minification and magnification
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, 0);
}
