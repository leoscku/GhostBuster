#include "Window.h"

const char* window_title = "GLFW Starter Project";
// Cube * cube;
GLint shaderProgram;
GLint terrainShaderProgram;
GLint skyboxShaderProgram;
GLint lineShaderProgram;

// On some systems you need to change this to the absolute path
#define VERTEX_SHADER_PATH "./shader.vert"
#define FRAGMENT_SHADER_PATH "./shader.frag"

// Default camera parameters
glm::vec3 Window::cam_pos(0.0f, 0.0f, 0.0f);		// e  | Position of camera
glm::vec3 cam_look_at(0.0f, 0.0f, 0.0f);	// d  | This is where the camera looks at
glm::vec3 cam_up(0.0f, 1.0f, 0.0f);			// up | What orientation "up" is

int Window::width;
int Window::height;

glm::mat4 Window::P;
glm::mat4 Window::V;

//Player
Player* player;
float lastX;
float lastY;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;  // time between current frame and last frame
float lastFrame = 0.0f;

glm::mat4 camRotMat = glm::mat4(1.0f);
glm::vec3 preVec;
bool lb_down = false;

// Use FMOD to create background music
FMOD_RESULT result;
FMOD::System *m_pSystem;
FMOD::Sound* Sound;
FMOD::Sound* gunSound;

Skybox* Window::skybox;
Island* island;
Bezier* bezier;
MatrixTransform * root;

void Window::initialize_objects()
{
  island = new Island();
  player = new Player(glm::vec3(0.0f, 0.0f, 0.0f), island);
    
    Window::skybox = new Skybox();
    bezier = new Bezier();
    
    root = new MatrixTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)));
    int waterSize = 20;
    for (int i = 0; i < waterSize*waterSize; i++) {
        int x = i % waterSize;
        int y = i / waterSize;
        MatrixTransform* temp = new MatrixTransform(glm::translate(glm::mat4(1.0f), glm::vec3((x- waterSize/2) * 200, 0, (y-  waterSize/2) * 200)) *
                                                    glm::scale(glm::mat4(1.0f), glm::vec3(200, 200, 200)));
        temp->addChild(bezier);
        root->addChild(temp);
    }

	// Load the shader program. Make sure you have the correct filepath up top
	shaderProgram = LoadShaders(VERTEX_SHADER_PATH, FRAGMENT_SHADER_PATH);
    terrainShaderProgram = LoadShaders("./terrainShader.vert", "./terrainShader.frag");
    skyboxShaderProgram = LoadShaders("./skyShader.vert", "./skyShader.frag");
    lineShaderProgram = LoadShaders("./lineShader.vert", "./lineShader.frag");

    if (FMOD::System_Create(&m_pSystem) != FMOD_OK)
    {
        // Report Error
        return;
    }
    
    int driverCount = 0;
    m_pSystem->getNumDrivers(&driverCount);
    
    if (driverCount == 0)
    {
        // Report Error
        return;
    }
    
    // Initialize our Instance with 36 Channels
    m_pSystem->init(36, FMOD_INIT_NORMAL, NULL);
    
    m_pSystem->createSound("test.mp3",FMOD_LOOP_NORMAL, 0, &Sound);
    //m_pSystem->createSound("gun.mp3",FMOD_INIT_NORMAL, 0, &gunSound);

    m_pSystem->playSound( Sound,NULL, false, 0);
}

// Treat this as a destructor function. Delete dynamically allocated memory here.
void Window::clean_up()
{
	// delete(cube);
	glDeleteProgram(shaderProgram);
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}
  
  lastX = width / 2.0f;
  lastY = height / 2.0f;

	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ // Because Apple hates comforming to standards
	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		fprintf(stderr, "Either GLFW is not installed or your graphics card does not support modern OpenGL.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Get the width and height of the framebuffer to properly resize the window
	glfwGetFramebufferSize(window, &width, &height);
	// Call the resize callback to make sure things get drawn immediately
	Window::resize_callback(window, width, height);

	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	glfwGetFramebufferSize(window, &width, &height); // In case your Mac has a retina display
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size. This is the only matrix that OpenGL maintains for us in modern OpenGL!
	glViewport(0, 0, width, height);

	if (height > 0)
	{
		P = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 10000.0f);
		V = glm::lookAt(cam_pos, cam_look_at, cam_up);
	}
}

void Window::idle_callback()
{
	// Call the update function the cube
	// cube->update();
    cam_pos = player -> getPosition();
    Window::V = player ->getViewMatrix();
    bezier->update();
}

void Window::display_callback(GLFWwindow* window)
{
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
  
  processInput(window);
  
	// Clear the color and depth buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the shader of programID
	glUseProgram(shaderProgram);
  player -> draw(shaderProgram);

  glUseProgram(terrainShaderProgram);
  island->draw(terrainShaderProgram, player -> getViewMatrix());

    glUseProgram(skyboxShaderProgram);
  skybox->draw(skyboxShaderProgram);
    root->draw(lineShaderProgram, glm::mat4(1.0f));
	// Gets events, including input such as keyboard and mouse or window resizing
	glfwPollEvents();
	// Swap buffers
	glfwSwapBuffers(window);
}

void Window::processInput(GLFWwindow *window) {
  
  //std::cout << "keyboard" << std::endl;
  
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
    player -> processKeyboard(FORWARD, deltaTime);
  }
  
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
    player -> processKeyboard(BACKWARD, deltaTime);
  }
  
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
    player -> processKeyboard(LEFT, deltaTime);
  }
  
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
    player -> processKeyboard(RIGHT, deltaTime);
  }
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
        
        if (key == GLFW_KEY_9){
            island->reGenerateData();
        }
	}
}

void Window::cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }
  
  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
  
  lastX = xpos;
  lastY = ypos;
  
  //std::cout << "mouse" << std::endl;
  player -> processMouseMovement(xoffset, yoffset);
}

void Window::mouse_callback(GLFWwindow* window, int button, int actions, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT){

        if(actions == GLFW_PRESS){

            double x, y;
            glfwGetCursorPos(window, &x, &y);
            preVec = calTrackBallVec(x, y);
            lb_down = true;
        }
        else if (actions == GLFW_RELEASE){
            lb_down = false;

        }
    }
}


glm::vec3 Window::calTrackBallVec(double x, double y){
    glm::vec3 v;    // Vector v is the synthesized 3D position of the mouse location on the trackball
    float d;     // this is the depth of the mouse location: the delta between the plane through the center of the trackball and the z position of the mouse
    v.x = (4.0 * x - width) / width;   // this calculates the mouse X position in trackball coordinates, which range from -1 to +1
    v.y = (height - 4.0 * y) / height;   // this does the equivalent to the above for the mouse Y position
    v.z = 0.0;   // initially the mouse z position is set to zero, but this will change below
    d = glm::length(v);    // this is the distance from the trackball's origin to the mouse location, without considering depth (=in the plane of the trackball's origin)
    d = (d < 1.0f) ? d : 1.0;   // this limits d to values of 1.0 or less to avoid square roots of negative values in the following line
    v.z = sqrtf(1.001f - d * d);  // this calculates the Z coordinate of the mouse position on the trackball, based on Pythagoras: v.z*v.z + d*d = 1*1
    v = glm::normalize(v); // Still need to normalize, since we only capped d, not v.
    return v;  // return the mouse location on the surface of the trackball
}

glm::vec3 Window::calRotateVec(glm::vec3 prev, glm::vec3 curr){
    return glm::normalize(glm::cross(prev, curr));
}
