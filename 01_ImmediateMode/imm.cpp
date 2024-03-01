// 01_ImmediateMode
// Immediate Mode Example using glfw

#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// window size
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    if (!glfwInit()) {
        std::cout << "Failed to initiallize GLFW" << std::endl;
        return -1;
    }; 

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "01_1_Immediate Mode", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    // we don't have to use GLEW library in this program
    // cause immediate mode doesn't need any modern OpenGL features (core profile)
    
    // main loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        
        // setting the camera
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        float ratio = SCR_WIDTH / (float)SCR_HEIGHT;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        // orthographic projection of NDC
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        // triangle (red)
        glColor3f(1.f, 0.f, 0.f);
        glBegin(GL_TRIANGLES);
            glVertex3f(-0.8f, -0.8f, 0.f);
            glVertex3f(-0.2f, -0.8f, 0.f);
            glVertex3f(-0.5f, -0.2f, 0.f);
        glEnd();
        
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
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
