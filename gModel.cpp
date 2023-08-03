#include "gModel.h"

Model::Model(std::string _modelId, const std::vector<std::shared_ptr<MeshInterface>> & _meshesPtrs)
{
    modelId    = _modelId;
    meshesPtr  = _meshesPtrs;
}

const std::string & Model::getModelId()
{
    return modelId;
}

const std::vector<std::shared_ptr<MeshInterface>> & Model::getMeshesPtr()
{
    return meshesPtr;
}

RenderObject::RenderObject(std::shared_ptr<Model> _modelPtr, const Motion & _motion)
{
    relatedModel = _modelPtr;
    motion       = _motion;
}

void RenderObject::SetPosition(glm::vec3 _position)
{
    motion.SetPosition(_position);
}

glm::vec3 RenderObject::GetPosition()
{
    return motion.getPosition();
}

std::shared_ptr<Model> RenderObject::GetRelatedModel()
{
    return relatedModel;
}

void RenderObject::Move(glm::vec3 direction, float speed, float deltaTime)
{
    glm::vec3 vecDist = direction * speed * deltaTime;
    motion.Move(vecDist);
}

glm::mat4 RenderObject::GetModelMatrix()
{
    return motion.getModelMatrix();
}