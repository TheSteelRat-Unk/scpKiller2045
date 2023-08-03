#include "WindowHandler.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


bool WindowHandler::InitGlfwContext() 
{
    try 
    {
        int success = glfwInit();
        if(!success)
            throw::std::runtime_error(" failed to init gwfw");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif
        return true;
    }
    catch(const std::exception & err)
    {
        std::cout << " err " << err.what() << std::endl;
        return false;
    }
}

bool WindowHandler::CreateWindow(int width, int height, const std::string title)
{
    try 
    {
        bool success = InitGlfwContext();
        if(!success)
            throw::std::runtime_error(" no chance to create window, \
            gl context or gl functions setting failed");

        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
        glfwMakeContextCurrent(window);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        return LoadGLfunctions();
    }
    catch(const std::exception & err)
    {
        std::cout << " err " << err.what() << std::endl;
        return false;
    }
}

bool WindowHandler::LoadGLfunctions()
{
    try
    {
        int success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        if(!success)
            throw::std::runtime_error(" failed to load glad func-s pointers");
        return true;
    }
    catch(const std::exception & err)
    {
        std::cout << " err " << err.what() << std::endl;
        return false;
    }
}

GLFWwindow* WindowHandler::GetWindow()
{
    return window;
}
