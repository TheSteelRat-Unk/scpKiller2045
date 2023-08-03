#include "gbaseVertexes.h"
#include <iostream>

BaseVertex::BaseVertex(glm::vec3 ver, glm::vec2 tex) : 
    vertice(ver), texCoords(tex) 
{ }

void BaseVertex::CopyThis(unsigned char * const Ptr)
{
    try 
    {
        if(Ptr)
        {
            std::memcpy(Ptr, &vertice, sizeof(glm::vec3));
            std::memcpy(Ptr + sizeof(glm::vec3), &texCoords, sizeof(glm::vec2));
        } else {
            throw::std::runtime_error(" null ptr in copy vertex func ");
        }
    }
    catch(const std::exception & err)
    {
        std::cout << " err " << err.what() << std::endl;
    }
}

format BaseVertex::GetFormat() //chech this 
{
    format f = {
        sizeof(float) * 5,
        std::list<std::pair<attribute, size_t>>
        {
            {std::make_pair(verticeAttr,     sizeof(float) * 3)},
            {std::make_pair(texcoordsAttr,   sizeof(float) * 2)},
        }
    };
    return f;
}

BaseVertex::~BaseVertex()
{ }


//****************

DefVertex::DefVertex(glm::vec3 ver, glm::vec2 tex,
                  glm::vec3 nor, glm::vec3 tan, glm::vec3 bitan): 
                  
                  BaseVertex(ver, tex), normals(nor), tangent(tan), bitangent(bitan)
{ }

void DefVertex::CopyThis(unsigned char * const Ptr)
{
    try
    {
        if(Ptr)
        {
            unsigned char* offset = Ptr;
            std::memcpy(offset, &vertice, sizeof(glm::vec3));
            offset = offset + sizeof(glm::vec3);

            std::memcpy(offset, &texCoords, sizeof(glm::vec2));
            offset = offset + sizeof(glm::vec2);

            std::memcpy(offset, &normals, sizeof(glm::vec3));
            offset = offset + sizeof(glm::vec3);

            std::memcpy(offset, &tangent, sizeof(glm::vec3));
            offset = offset + sizeof(glm::vec3);

            std::memcpy(offset, &bitangent, sizeof(glm::vec3));
            offset = offset + sizeof(glm::vec3);

        } else {
            throw::std::runtime_error(" null ptr in copy vertex func ");
        }
    }
    catch(const std::exception & err)
    {
        std::cout << " err " << err.what() << std::endl;
    }
}

format DefVertex::GetFormat() 
{
    format f = {
        sizeof(float) * 14,
        std::list<std::pair<attribute, size_t>>
        {
            {std::make_pair(verticeAttr,     sizeof(float) * 3)},
            {std::make_pair(texcoordsAttr,   sizeof(float) * 2)},
            {std::make_pair(normalAttr,      sizeof(float) * 3)},
            {std::make_pair(tangentAttr,     sizeof(float) * 3)},
            {std::make_pair(bitangentAttr,   sizeof(float) * 3)},
        }
    };
    return f;
}

DefVertex::~DefVertex()
{ }
