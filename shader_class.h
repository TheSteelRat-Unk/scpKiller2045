#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
    public:
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath);

        Shader() { }
       
        void use()
        {
            glUseProgram(ID);
        }
       
        void SetBool(const std::string &name, bool value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
        }
        void SetInt(const std::string &name, int value) const
        {
            glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
        }
        void SetFloat(const std::string &name, float value) const 
        {
            glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
        }
        void SetFloatVec(const std::string &name, const std::vector<float> &value) const 
        {
            glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, value.data());
        }
        void SetMat4(const std::string &name, const glm::mat4 &matrix) const 
        {
            unsigned int matrixLoc = glGetUniformLocation(ID, name.c_str());
            glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, &matrix[0][0]);
        }
    
    private:
       
        void checkCompileErrors(unsigned int shader, std::string type);
};