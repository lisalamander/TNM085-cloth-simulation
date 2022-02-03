#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <iostream>
class Texture
{
public:
    Texture(char const* path) {
        loadTexture(path);
    }
    unsigned int getID() const { return textureID; };
private:
    void loadTexture(char const* path);
    unsigned int textureID;
};

