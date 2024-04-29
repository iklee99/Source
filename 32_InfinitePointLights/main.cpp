// 32_InfinitePointLights
//      Mouse: Arcball manipulation
//      Keyboard: 'r' - reset arcball
//                'a' - toggle camera/object rotation

#include <GL/glew.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>

#include <shader.h>
#include <cube.h>
#include <arcball.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
 

using namespace std; 

// Function Prototypes
GLFWwindow *glAllInit();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action , int mods);
void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow *window, double x, double y);
unsigned int loadTexture(const char *);
void render();

// Global variables
GLFWwindow *mainWindow = NULL;
Shader *lightingShader = NULL;
Shader *lampShader = NULL;
unsigned int SCR_WIDTH = 600;
unsigned int SCR_HEIGHT = 600;
Cube *cube;
Cube *lamp;
glm::mat4 projection, view, model;

// for arcball
float arcballSpeed = 0.2f;
static Arcball camArcBall(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true );
static Arcball modelArcBall(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
bool arcballCamRot = true;

// for camera
glm::vec3 cameraPos(0.0f, 0.0f, 9.0f);

// for lighting
glm::vec3 lightSize(0.1f, 0.1f, 0.1f);
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3( -1.5f,  0.2f,  1.0f),
    glm::vec3( 2.0f, -1.3f, -1.0f)
};

// for texture
static unsigned int diffuseMap, specularMap;  // texture ids for diffuse and specular maps


int main()
{
    mainWindow = glAllInit();
    
    // shader loading and compile (by calling the constructor)
    lightingShader = new Shader("6.multiple_lights.vs", "6.multiple_lights.fs");
    lampShader = new Shader("6.lamp.vs", "6.lamp.fs");
    
    // projection and view matrix
    lightingShader->use();
    projection = glm::perspective(glm::radians(45.0f),
                                  (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    lightingShader->setMat4("projection", projection);
    
    lampShader->use();
    lampShader->setMat4("projection", projection);
    
    // load texture
    diffuseMap = loadTexture("container2.bmp");
    specularMap = loadTexture("container2_specular.bmp");
    
    // transfer texture id to fragment shader
    lightingShader->use();
    lightingShader->setInt("material.diffuse", 0);
    lightingShader->setInt("material.specular", 1);
    lightingShader->setFloat("material.shininess", 32);
    
    lightingShader->setVec3("viewPos", cameraPos);
    
    // transfer lighting parameters to fragment shader
    // directional light
    lightingShader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    lightingShader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    lightingShader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    lightingShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    lightingShader->setVec3("pointLights[0].position", pointLightPositions[0]);
    lightingShader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    lightingShader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    lightingShader->setFloat("pointLights[0].constant", 1.0f);
    lightingShader->setFloat("pointLights[0].linear", 0.09);
    lightingShader->setFloat("pointLights[0].quadratic", 0.032);
    // point light 2
    lightingShader->setVec3("pointLights[1].position", pointLightPositions[1]);
    lightingShader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    lightingShader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    lightingShader->setFloat("pointLights[1].constant", 1.0f);
    lightingShader->setFloat("pointLights[1].linear", 0.09);
    lightingShader->setFloat("pointLights[1].quadratic", 0.032);
    
    // create a cubes
    cube = new Cube();
    
    while (!glfwWindowShouldClose(mainWindow)) {
        render();
        glfwPollEvents();
    }
    
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
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Infinite and Point Lights", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    
    // OpenGL states
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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

unsigned int loadTexture(const char *texFileName) {
    unsigned int texture;
    
    // Create texture ids.
    glGenTextures(1, &texture);
    
    // All upcomming GL_TEXTURE_2D operations now on "texture" object
    glBindTexture(GL_TEXTURE_2D, texture);
    
    // Set texture parameters for wrapping.
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);   // vertical flip the texture
    unsigned char *image = stbi_load(texFileName, &width, &height, &nrChannels, 0);
    if (!image) {
        printf("texture %s loading error ... \n", texFileName);
    }
    else printf("texture %s loaded\n", texFileName);
    
    GLenum format;
    if (nrChannels == 1) format = GL_RED;
    else if (nrChannels == 3) format = GL_RGB;
    else if (nrChannels == 4) format = GL_RGBA;
    
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap(GL_TEXTURE_2D);
    
    return texture;
}

void render() {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    view = view * camArcBall.createRotationMatrix();
    
    // cube objects
    lightingShader->use();
    lightingShader->setMat4("view", view);
    
    // texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
    
    // cube1
    model = glm::mat4(1.0f);
    model = model * modelArcBall.createRotationMatrix();
    lightingShader->setMat4("model", model);
    cube->draw(lightingShader);
    
    // cube2
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0f, -1.0f, -1.0f));
    lightingShader->setMat4("model", model);
    cube->draw(lightingShader);
    
    // cube3
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.5f, 2.0f, 1.0f));
    lightingShader->setMat4("model", model);
    cube->draw(lightingShader);
    
    // lamps (point lights)
    lampShader->use();
    lampShader->setMat4("view", view);
    for (int i = 0; i < 2; i++) {
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[i]);
        model = glm::scale(model, lightSize);
        lampShader->setMat4("model", model);
        cube->draw(lampShader);
    }
    
    glfwSwapBuffers(mainWindow);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        camArcBall.init(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
        modelArcBall.init(SCR_WIDTH, SCR_HEIGHT, arcballSpeed, true, true);
    }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS) {
        arcballCamRot = !arcballCamRot;
        if (arcballCamRot) {
            cout << "ARCBALL: Camera rotation mode" << endl;
        }
        else {
            cout << "ARCBALL: Model  rotation mode" << endl;
        }
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (arcballCamRot)
        camArcBall.mouseButtonCallback( window, button, action, mods );
    else
        modelArcBall.mouseButtonCallback( window, button, action, mods );
}

void cursor_position_callback(GLFWwindow *window, double x, double y) {
    if (arcballCamRot)
        camArcBall.cursorCallback( window, x, y );
    else
        modelArcBall.cursorCallback( window, x, y );
}
