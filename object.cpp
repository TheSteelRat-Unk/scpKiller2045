#include "Object.h"


void Object::Move(glm::vec3 direction, float speed, float deltaTime)
{
    glm::vec3 shift = speed * deltaTime * direction;
    motion.Move(shift);
}

void Object::Rotate(glm::vec3 axis, float speed, float deltaTime)
{
    float angle = speed * deltaTime;
    motion.Rotate(angle, axis);
}

void Object::SetPosition(glm::vec3 position)
{
    motion.SetPosition(position);
}

glm::vec3 Object::getPosition()
{
    return motion.getPosition();
}

void Object::AddMesh(std::shared_ptr<MeshInterface> mesh)
{
    meshes.push_back(mesh);
}

void Object::DrawObject(Shader & shader)
{
    shader.use();
    shader.SetMat4("model", motion.getModelMatrix());

    //std::cout << " ТАКС " << std::endl;

    for(auto & mesh : meshes)
    {
        mesh -> getVAO().bind();
        mesh -> getVAO().drawIndices();
    }
}