#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "includes/Utilities.hpp"
#include "includes/Shader.h"

#include "includes/TriangleSoup.hpp"
#include <iostream>
// Initial screen dimensions
const int SCR_WIDTH = 400;
const int SCR_HEIGHT = 400;
const float M_PI = 3.1415927;

void processInput(GLFWwindow* window);

int main() {
    // Initialize glfw
    glfwInit();
    // Tell glfw which version of OpenGL we are using
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Create a window object

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Terrain generator v3", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // Print nr of attributes supported
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
  // Create shader
    Shader myShader("shaders/vertex.glsl", "shaders/fragment.glsl");
    myShader.use();
    glm::mat4 projection = glm::perspective((float)M_PI / 4.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);
    myShader.setMat4("projection", projection);
    // Create sphere object
    TriangleSoup sphere;
    sphere.createSphere(0.5, 30);
    


    while (!glfwWindowShouldClose(window))
    {
        util::displayFPS(window);
        // Keyboard input
        processInput(window);
        // Cursor position
        
        // rendering commands
        //glEnable(GL_BLEND);
        //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_CULL_FACE);
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0, 0, -10.0f));
        myShader.use();
        myShader.setMat4("model", model);
        myShader.setMat4("view", glm::mat4(1.0f));
        
        sphere.render();
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glBindVertexArray(0);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
