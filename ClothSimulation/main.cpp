#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include "includes/Texture.h"
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
#include "RenderEnviro.h"
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
    float deltaTime = 0;
    float lastTime = 0;
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
   
    Shader sphereShader("shaders/vertex_sphere.glsl", "shaders/fragment_sphere.glsl");

    // Projection matrix
    glm::mat4 projection = glm::perspective(M_PI / 4.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);

    myShader.use();
    myShader.setMat4("projection", projection);
    sphereShader.use();
    sphereShader.setMat4("projection", projection);

    // Create sphere object
    TriangleSoup sphere;
    sphere.createSphere(sphereRadius, 30);
    glm::vec3 spherePosition = glm::vec3(-1.0f, 0.0f, -5.0f);

    Cloth myCloth(50, 50, glm::vec3(0.0f, 0.0f, -5.0f));
    
    // Stuff for cameramovement

    util::KeyRotator myKeyRotator(window);
    util::MouseRotator myMouseRotator(window);
    glm::mat4 cameraMove = glm::mat4(1.0f);
    glm::vec3 cameraPosition = glm::vec3(0,0,0);

    
    // Texture settings
    Texture texture("images/cloth_texture.png");
    myShader.setInt("texture_", 0);   
   
    Texture floorTex("images/grass.jpg");
    myShader.setInt("texture_", 1);

    Texture balkTex("images/wooden.jpg");
    myShader.setInt("texture_", 2);
   
    

    while (!glfwWindowShouldClose(window))
    {
        util::displayFPS(window);
        // Keyboard input
        processInput(window);
        
        // Update time
        deltaTime = glfwGetTime() - lastTime;
        lastTime = glfwGetTime();
        // rendering commands

        //Get input
        myMouseRotator.poll();
        cameraPosition =  myKeyRotator.move();

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glEnable(GL_CULL_FACE);

        //Camera
        
        cameraMove = glm::translate(cameraMove, glm::vec3(cameraPosition.x,cameraPosition.y,cameraPosition.z));
       
                
        //render Enviro
        renderEnviroment(myShader, floorTex, cameraMove, balkTex);

        //Sphere
        glm::mat4 model = glm::mat4(1.0f);
        glm::vec3 mouseDelta = glm::vec3(myMouseRotator.delta()/100.0f);
        spherePosition += mouseDelta;
        model = glm::translate(model, glm::vec3(spherePosition.x, spherePosition.y, spherePosition.z));

        sphereShader.use();
        sphereShader.setMat4("model", model);
        sphereShader.setMat4("view", cameraMove);
        
        myCloth.updateSimulation(0.003); 
        myCloth.handleSphereIntersections(sphereRadius, spherePosition);
        
        myCloth.updateBuffers();

      /*  myCloth2.updateSimulation(0.005);
        myCloth2.handleSphereIntersections(sphereRadius, spherePosition);
        myCloth2.updateBuffers();*/
        sphere.render();
        model = glm::mat4(1.0f);
        //model = glm::translate(model, glm::vec3(myCloth.getPos()));
        myShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture.getID());
        myShader.setMat4("model", model);
        myShader.setMat4("view", cameraMove);
        myCloth.render();
        //myCloth2.render();
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

