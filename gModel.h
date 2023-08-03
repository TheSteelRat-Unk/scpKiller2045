#pragma once
#include <iostream>
#include "gmesh.h"
#include "gmotion.h"

class Model
{
    public:
        Model(const std::string path); 
        Model(std::string _modelId, const std::vector<std::shared_ptr<MeshInterface>> & _meshesPtr); 


        const std::string & getModelId();

        const std::vector<std::shared_ptr<MeshInterface>> & getMeshesPtr(); 
    private: 
        std::string modelId;
        std::vector<std::shared_ptr<MeshInterface>> meshesPtr;
};

class RenderObject
{
    public: // may be const better, after copy it will be ok
        RenderObject(std::shared_ptr<Model> _modelPtr, const Motion & _motion);

        void        Move       (glm::vec3 _direction, float speed, float deltaTime);
        void        SetPosition(glm::vec3 _position);

        glm::vec3   GetPosition();
        glm::mat4   GetModelMatrix();

        std::shared_ptr<Model> GetRelatedModel();

    private:
        Motion motion;
        std::shared_ptr<Model> relatedModel;
};