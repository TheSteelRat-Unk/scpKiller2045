#include "gCamera.h"

Camera::Camera(const glm::vec3 _position, const float _speed,
               const float _sensitivity, GLFWwindow* window):

position(_position), speed(_speed), sensitivity(_sensitivity)
{
    glfwSetWindowUserPointer(window, this); 
    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xposIn, double yposIn) 
    {
        static_cast<Camera*>(glfwGetWindowUserPointer(window)) -> mouse_callback(window, xposIn, yposIn);
    });

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    forward     = glm::vec3  (0.0, 0.0, -1.0);
    LookAt      = glm::lookAt(position, position + forward, up);
}

Camera::Camera(const glm::vec3 _position, const float _speed) :
position(_position), speed(_speed)
{
    forward     = glm::vec3  (0.0, 0.0, -1.0);
    LookAt      = glm::lookAt(position, position + forward, up);
}

void Camera::AdjustWithTime(float time)
{
    speed = float(time * 3.0f);
    //std::cout << "скорость" << speed << std::endl;
}

void Camera::UpdateMove(glm::vec3 directionVec)
{
    position += directionVec * speed;
    glm::vec3 viewPoint = position + forward;

    LookAt      = glm::lookAt(position, viewPoint, up);
}

void Camera::UpdateForward()
{
    glm::vec3 directionVec = forward;
    UpdateMove(directionVec);
}

void Camera::UpdateBack()
{
    glm::vec3 directionVec = -forward; 
    UpdateMove(directionVec);
}

void Camera::UpdateLeft()
{
    glm::vec3 directionVec = glm::normalize(glm::cross(up, forward));
    UpdateMove(directionVec);
}

void Camera::UpdateRight()
{
    glm::vec3 directionVec = glm::normalize(glm::cross(forward, up));
    UpdateMove(directionVec);
}

glm::mat4 Camera::GetView()
{
    return LookAt;
}

glm::vec3 Camera::GetCameraPosition()
{
    return position;
}


void Camera::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstTime)
    {
        lastX = xpos;
        lastY = ypos;
        firstTime = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    forward = glm::normalize(front);
    LookAt  = glm::lookAt(position, position + forward, up);
}


