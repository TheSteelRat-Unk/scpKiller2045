#define pragma once
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Camera
{
    public: 
        Camera(const glm::vec3 _position,  
               const float _speed       );

        Camera(const glm::vec3 _position, const float _speed,
                       const float sensitivity,   GLFWwindow* window );


        void UpdateMove(glm::vec3 directionVec);
        void AdjustWithTime(float time);

        void UpdateForward();
        void UpdateLeft();
        void UpdateRight();
        void UpdateBack();

        glm::mat4 GetView();
        glm::vec3 GetCameraPosition();

    private:
        glm::vec3 position;
        glm::vec3 forward;

        glm::mat4 LookAt;
        float      speed;
        float sensitivity;

        void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

        const glm::vec3 up     = glm::vec3(0.0, 1.0, 0.0);
        bool  firstTime        = true;
        float yaw              = -90.0f;	
        float pitch            =  0.0f;

        float lastX =  800 / 2.0; //1024
        float lastY =  600 /  2.0; //1024
};