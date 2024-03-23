// RotArbitAxis
// Keyboard: q: rotation by quaterion
//           m: rotation by matrix
//           ESC: quit

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <cmath>
#include <shader.h>
#include <cube.h>

using namespace std;

// Function Prototypes
GLFWwindow *glAllInit();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action , int mods);
void render();

// Global variables
GLFWwindow *window = NULL;
Shader *globalShader = NULL;
unsigned int SCR_WIDTH = 600;
unsigned int SCR_HEIGHT = 600;
Cube *cube;
float lastTime = 0.0f;
glm::vec3 axis(1.0f, 1.0f, 1.0f);
glm::vec3 cubeCenter(0.4f, 0.4f, 0.0f);
float cubeScale = 0.3f;
enum {MAT_ROT, QUAT_ROT} rotState = MAT_ROT;


int main()
{
    window = glAllInit();

    // shader loading and compile (by calling the constructor)
    globalShader = new Shader("4.3.transform.vs", "4.3.transform.fs");
    
    // cube initialization
    cube = new Cube(cubeCenter[0], cubeCenter[1], cubeCenter[2], cubeScale);
    
    // normalizing rotation axis
    axis = glm::normalize(axis);
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window)) {
        render();
        glfwPollEvents();
    }
    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

GLFWwindow *glAllInit()
{
    GLFWwindow *window;
    
    // glfw: initialize and configure
    if (!glfwInit()) {
        printf("GLFW initialisation failed!");
        glfwTerminate();
        exit(-1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    // glfw window creation
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Rotation Axis", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    
    // OpenGL states
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    
    // Allow modern extension features
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cout << "GLEW initialisation failed!" << endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        exit(-1);
    }
    
    return window;
}

void render()
{
    glm::mat4 transform;
    float currentTime = glfwGetTime();
    float deltaTime = currentTime - lastTime;  // in seconds (in glfw)
    float angleStepDeg = (float)90.0f * deltaTime;
    float angleStepRad = glm::radians(angleStepDeg);
    
    globalShader->use();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    transform = glm::mat4(1.0f); // initialize matrix to identity matrix
    
    if (rotState == MAT_ROT) {
        transform = glm::rotate(transform, angleStepRad, axis);
    }
    else if (rotState == QUAT_ROT) {
        glm::quat q = glm::angleAxis(angleStepRad, axis);
        glm::mat4 qrot = glm::toMat4(q);
        transform = transform * qrot;
    }
    
    globalShader->setMat4("transform", transform);
    
    cube->draw(globalShader);
    
    glfwSwapBuffers(window);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_HEIGHT = height;
    SCR_WIDTH = width;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        rotState = QUAT_ROT;
        cout << "rotaion by quaternion" << endl;
    }
    else if (key == GLFW_KEY_M && action == GLFW_PRESS) {
        rotState = MAT_ROT;
        cout << "rotation by matrix" << endl;
    }
}

