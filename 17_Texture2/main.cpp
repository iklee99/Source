// Texture2
//    Keyboard: space_bar: texture mode 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, ...

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>

#include <shader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;

// Function Prototypes
GLFWwindow *glAllInit();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action , int mods);
void loadTexture();
void genRectangle();
void render();

// Global variables
GLFWwindow *mainWindow = NULL;
Shader *globalShader = NULL;
unsigned int SCR_WIDTH = 600;
unsigned int SCR_HEIGHT = 600;
static float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
    -0.5f,  0.5f, 0.0f
};
static float colors[] = {
    1, 0, 0, 1,
    1, 1, 0, 1,
    0, 1, 0, 1,
    0, 1, 1, 1
};
static float texcoord[] = {
     0.0f,  0.0f,
     1.0f,  0.0f,
     1.0f,  1.0f,
     0.0f,  1.0f
};
unsigned int VAO;
unsigned int VBO;
unsigned int vSize, cSize, tSize;
int texCoordState = 0;

// for texture
static unsigned int texture1; // Array of texture ids.


int main()
{
    mainWindow = glAllInit();
    
    // shader loading and compile (by calling the constructor)
    globalShader = new Shader("global.vs", "global.fs");
    
    // projection and view matrix
    globalShader->use();
    
    // load texture
    loadTexture();
    
    // generating rectangle
    vSize = sizeof(vertices);
    cSize = sizeof(colors);
    tSize = sizeof(texcoord);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    genRectangle();
    
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
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Texture 2", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    
    // OpenGL states
    glClearColor(0.5f, 0.6f, 0.6f, 1.0f);
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

void loadTexture() {
    
    // Create texture ids.
    glGenTextures(1, &texture1);
    
    // All upcomming GL_TEXTURE_2D operations now on "texture" object
    glBindTexture(GL_TEXTURE_2D, texture1);
    
    // Set texture parameters for wrapping.
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);   // vertical flip the texture
    unsigned char *image = stbi_load("launch.bmp", &width, &height, &nrChannels, 0);
    if (!image) {
        printf("texture %s loading error ... \n", "launch.bmp");
    }
    else printf("texture %s loaded\n", "launch.bmp");
    
    GLenum format;
    if (nrChannels == 1) format = GL_RED;
    else if (nrChannels == 3) format = GL_RGB;
    else if (nrChannels == 4) format = GL_RGBA;
    
    glBindTexture( GL_TEXTURE_2D, texture1 );
    glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image );
    glGenerateMipmap(GL_TEXTURE_2D);
    
}

void genRectangle() {
    
    glBindVertexArray(VAO);
    
    // copy vertex attrib data to VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vSize+cSize+tSize, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vSize, vertices);    // copy verts at offset 0
    glBufferSubData(GL_ARRAY_BUFFER, vSize, cSize, colors);
    glBufferSubData(GL_ARRAY_BUFFER, vSize+cSize, tSize, texcoord);
    
    // attribute position initialization
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  // position attrib
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)vSize);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)(vSize+cSize));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void render() {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    globalShader->use();
    
    glBindTexture(GL_TEXTURE_2D, texture1);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
    
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
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        int m = 4;
        texCoordState = (texCoordState + 1) % m;
        
        switch (texCoordState) {
            case 0:
                texcoord[0] = 0.0f; texcoord[1] = 0.0f;
                texcoord[2] = 1.0f; texcoord[3] = 0.0f;
                texcoord[4] = 1.0f; texcoord[5] = 1.0f;
                texcoord[6] = 0.0f; texcoord[7] = 1.0f;
                genRectangle();
                break;
            case 1:
                texcoord[0] = 0.0f; texcoord[1] = 0.0f;
                texcoord[2] = 0.5f; texcoord[3] = 0.0f;
                texcoord[4] = 0.5f; texcoord[5] = 0.5f;
                texcoord[6] = 0.0f; texcoord[7] = 0.5f;
                genRectangle();
                break;
            case 2:
                texcoord[0] = -1.0f; texcoord[1] = 0.0f;
                texcoord[2] = 2.0f; texcoord[3] = 0.0f;
                texcoord[4] = 2.0f; texcoord[5] = 1.0f;
                texcoord[6] = -1.0f; texcoord[7] = 1.0f;
                genRectangle();
                break;
            case 3:
                texcoord[0] = -1.0f; texcoord[1] = -1.0f;
                texcoord[2] = 2.0f; texcoord[3] = -1.0f;
                texcoord[4] = 2.0f; texcoord[5] = 2.0f;
                texcoord[6] = -1.0f; texcoord[7] = 2.0f;
                genRectangle();
                break;
                
        }
    }
}
