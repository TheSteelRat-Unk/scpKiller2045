#pragma once 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gmesh.h"
#include "gmotion.h" 
#include "shader_class.h"

class Object
{
    public:
        Object(const Motion & _motion, const std::string & _id) : motion(_motion), nameId(_id) { }

        //~Object();
        void DrawObject(Shader & shader);

        void Move   (glm::vec3 direction, float speed, float deltaTime);
        void Rotate (glm::vec3 axis,      float speed, float deltaTime);

        glm::vec3 getPosition();

        void SetPosition(glm::vec3 position);

        void ObjectUpdate();
        void AddMesh(std::shared_ptr<MeshInterface> mesh);


        int getSize() { return meshes.size(); }

        //  const
        std::string GetId() { return nameId; }

    private:
        std::string nameId;

        std::vector<std::shared_ptr<MeshInterface>> meshes;
        Motion motion; 
};
