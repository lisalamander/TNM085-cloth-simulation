#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "includes/Utilities.hpp"
#include "includes/Shader.h"

#include "includes/TriangleSoup.hpp"
#include "includes/Cloth.h"
#include <iostream>
// #include <math.h>

//Egna 
#include "includes/Node.h"
// Initial screen dimensions
const int SCR_WIDTH = 1920/2;
const int SCR_HEIGHT = 1080/2;
const float M_PI = 3.1415927;
const float sphereRadius = 0.5f;
void processInput(GLFWwindow* window);

glm::mat4 mat4rotx(float angle);
glm::mat4 mat4roty(float angle);
glm::mat4 mat4rotz(float angle);


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

    glm::mat4 projection = glm::perspective(M_PI / 4.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);


    myShader.setMat4("projection", projection);
 
    // Create sphere object
    TriangleSoup sphere;
    sphere.createSphere(sphereRadius, 30);
    glm::vec3 spherePosition = glm::vec3(0, 0, -10);
    Cloth myCloth(10, 10, glm::vec3(0, 0, -5));
    
    // Stuff for cameramovement

    util::KeyRotator myKeyRotator(window);
    util::MouseRotator myMouseRotator(window);
    glm::mat4 cameraMove = glm::mat4(0.0f);
    glm::vec3 cameraPosition = glm::vec3(0,0,0);

    

    while (!glfwWindowShouldClose(window))
    {
        util::displayFPS(window);
        // Keyboard input
        processInput(window);
        
        // rendering commands

        //Get input
        myMouseRotator.poll();
        cameraPosition +=  myKeyRotator.move();

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_CULL_FACE);

        //Camera
        cameraMove = mat4rotz(myKeyRotator.theta()) * mat4roty(myKeyRotator.phi());
        cameraMove = glm::translate(cameraMove, glm::vec3(cameraPosition.x,cameraPosition.y,cameraPosition.z));

        glm::mat4 model = glm::mat4(1.0f);
        glm::vec3 mouseDelta = glm::vec3(myMouseRotator.delta()/100.0f);
        spherePosition += mouseDelta;
        model = glm::translate(model, glm::vec3(spherePosition.x, spherePosition.y, spherePosition.z));

        myShader.use();
        myShader.setMat4("model", model);
        myShader.setMat4("view", cameraMove);
        myCloth.updateSimulation(0.01);
        myCloth.handleSphereIntersections(sphereRadius, spherePosition);
        sphere.render();
        model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(myCloth.getPos()));
        myShader.use();
        myShader.setMat4("model", model);

        myCloth.render();
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
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

glm::mat4 mat4rotx(float angle) {


    glm::mat4 rotateX = {
         1.0f, 0.0f,       0.0f,        0.0f,
         0.0f, cos(angle), sin(angle), 0.0f,
         0.0f, -sin(angle), cos(angle),  0.0f,
         0.0f, 0.0f,       0.0f,        1.0f };



    return rotateX;

}

glm::mat4 mat4roty(float angle) {

    glm::mat4 rotateY = {
    cos(angle), 0.0f, -sin(angle), 0.0f,
    0.0f,       1.0f,  0.0f,       0.0f,
    sin(angle), 0.0f, cos(angle),0.0f,
    0.0f,       0.0f,  0.0f,       1.0f };

    return rotateY;


}

glm::mat4 mat4rotz(float angle) {

    glm::mat4 rotateZ = {
        cos(angle), sin(angle),  0.0f, 0.0f,
       -sin(angle), cos(angle),  0.0f, 0.0f,
        0.0f,        0.0f,       1.0f, 0.0f,
        0.0f,        0.0f,       0.0f, 1.0f };

    return rotateZ;
}

