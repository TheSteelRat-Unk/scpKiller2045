#pragma once
#include <iostream>
#include <map>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"


class Texture
{  

    public: 
        Texture(const std::string & filepath);
        void Bind();
        void Unbind();

        int GetWidth();
        int GetHeight();

    private:
        uint textureId;
        std::string name;
        int width;
        int height;

        int nrChannels;
        //char* data; // usually nullptr
};
