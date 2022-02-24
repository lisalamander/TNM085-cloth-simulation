#include "includes/RenderEnviro.h"



void renderEnviroment(Shader& myShader, Texture& floorTex, glm::mat4 cameraMove)
{

    //Create floor
    TriangleSoup floor;
    floor.createBox(2, 0.005, 2);
    glm::vec3 floorPosition = glm::vec3(0.0f, -1.0f, -5.0f);

    //Create tyghållare
    TriangleSoup balk;
    balk.createBox(0.05, 1, 0.05);
    glm::vec3 balkPosition = glm::vec3(0.0f, 0.0f, -5.0f);

    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(floorPosition.x, floorPosition.y, floorPosition.z));

    myShader.use();
    glActiveTexture(1);
    glBindTexture(GL_TEXTURE_2D, floorTex.getID());
    myShader.setMat4("model", model);
    myShader.setMat4("view", cameraMove);
    floor.render();

    //balk
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(balkPosition.x, balkPosition.y, balkPosition.z));

    myShader.use();
    myShader.setMat4("model", model);
    myShader.setMat4("view", cameraMove);
    balk.render();

    //balk2
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(balkPosition.x + 1.0f, balkPosition.y, balkPosition.z));

    myShader.use();
    myShader.setMat4("model", model);
    myShader.setMat4("view", cameraMove);
    balk.render();


}