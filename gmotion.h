#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

//  ~~
class Motion 
{
    public:
        Motion() : position(0.0f), angle(0.0f), axis(0.0, 1.0f, 0.0f)
        { 

        }
            //modelMatrix = glm::mat4(1.0f)  
        Motion(const glm::vec3 _position) : position(_position), angle(0.0f), axis(0.0f, 1.0f, 0.0f)
        { 

        }

        void Move(glm::vec3 translation)
        {
            position += translation;
        }

        void SetPosition(glm::vec3 _position)
        {
            position = _position;
        }
        // вот так нельзя, наверное
        void Rotate(float _angle, glm::vec3 _axis)
        {
            angle += _angle;
            axis  = _axis;
        }
        glm::mat4 getModelMatrix()
        {
            glm::mat4 modelMatrix = glm::mat4(1.0f);

            modelMatrix = glm::translate(modelMatrix, glm::vec3(position));
            modelMatrix = glm::rotate   (modelMatrix, glm::radians(angle), axis);

            return modelMatrix;
        }

        glm::vec3 getPosition()
        {
            return position;
        }

    private:
        glm::mat4  modelMatrix;
        glm::vec3     position;

        glm::vec3     axis;
        float        angle;
};

