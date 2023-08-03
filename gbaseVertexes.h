#pragma once 
#include "format.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class BaseVertex
{
    public:
        BaseVertex(glm::vec3 ver, glm::vec2 tex);
        ~BaseVertex();

        virtual format GetFormat();
        virtual void   CopyThis(unsigned char* const Ptr);

        glm::vec3 vertice;
        glm::vec2 texCoords;
};

class DefVertex : public BaseVertex
{
    public:
        DefVertex(glm::vec3 ver, glm::vec2 tex,
                  glm::vec3 nor, glm::vec3 tan, glm::vec3 bitan);
        ~DefVertex();

        format GetFormat() override;
        void   CopyThis(unsigned char * const Ptr) override;

        glm::vec3 normals;
        glm::vec3 tangent;
        glm::vec3 bitangent;
};