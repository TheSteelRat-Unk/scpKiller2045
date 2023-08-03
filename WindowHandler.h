#pragma once
#include <iostream>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Again, need logger & error handler

class WindowHandler
{
    public:
        WindowHandler()  { }
        ~WindowHandler() { }

        bool CreateWindow(int width, int height, std::string title);
        GLFWwindow* GetWindow();

    private:
        bool InitGlfwContext();
        bool LoadGLfunctions();

        GLFWwindow* window;
};
