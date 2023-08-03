#include "Loader.h"

std::vector<std::shared_ptr<MeshInterface>> Loader::ReturnData()
{
    return std::move(meshes);
}

void Loader::LoadModel(std::string path)
{
    scene = importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
         std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
         return;
    }

    MaterialExist = scene -> HasMaterials();

    ProcessNode(scene -> mRootNode, 0, 0);
}

void Loader::ProcessNode(aiNode* node, int depth, int i)
{
    int numChildren = node->mNumChildren; 
    int meshNumber  = node->mNumMeshes;

    std::cout << "=======================" << std::endl;
    std::cout << " Name " << node->mName.C_Str() << " Num Child: " << numChildren << std::endl; //Debug
    std::cout << " Depth " << depth << " X coord " << i << std::endl; //Debug
  

    std::cout << " Meshes to process: " << meshNumber << std::endl;
    for(unsigned int j = 0; j < meshNumber; j++) {
        // get mesh-assimp real ID and load Attrib (mesh class process it)
        int meshID = node->mMeshes[j];
      
        std::shared_ptr<MeshInterface> mesh = CreateMeshWithData(meshID); 
        // add mesh class obj pointer to vector + connect with correct node
        meshes.push_back(std::move(mesh));
    }

    for(unsigned int i = 0; i < numChildren; i++) {
        ProcessNode(node->mChildren[i], depth + 1, i);
    }
}

std::shared_ptr<MeshInterface> Loader::CreateMeshWithData(int meshID){

    std::vector<DefVertex> vData; 
    std::vector<uint> indices;
    //std::vector<Texture> textures; 

    aiMesh* mesh = scene->mMeshes[meshID];
    
    for (int i = 0; i < mesh->mNumVertices; i++) {
        vData.emplace_back(DefVertex {

            glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),

            mesh->mTextureCoords[0] != nullptr ? 
            glm::vec2 { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y } :             glm::vec3{},

            mesh->HasNormals() ? 
            glm::vec3{ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z } :           glm::vec3{},

            mesh->HasTangentsAndBitangents() ? 
            glm::vec3 { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z } :       glm::vec3{},

            mesh->HasTangentsAndBitangents() ? 
            glm::vec3 { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z } : glm::vec3{}
        });
    }
    // Индексы
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++){
            indices.push_back(face.mIndices[j]);       
        }
    }
    uint matID = mesh -> mMaterialIndex;
    aiMaterial* material = scene->mMaterials[matID];  

    //TO DO params 

    /*
    for(TextureType value = DIFFUSE; value <= UNKNOWN; value = static_cast<TextureType>
    (static_cast<int>(value) + 1)) {
        std::cout << " обрабатывается тип " << ConvertToStringTType(value);
        std::vector<Texture> typeTextures= GetTextureMaterialPositions(material, value, meshID);
        textures.insert(textures.end(), typeTextures.begin(), typeTextures.end());
    }
    */

    std::shared_ptr<MeshInterface> retMesh = std::make_shared<Mesh<DefVertex>>(vData, indices);
    return retMesh;
}
/*
Loader::Loader(std::list<Texture> & _textureCollection) : textureCollection(_textureCollection)
{

}
*/
Loader::Loader()
{

}

Loader::~Loader() 
{

}
