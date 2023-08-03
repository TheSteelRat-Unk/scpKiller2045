#pragma once 
#include <iostream>
#include <map>

#include "stb_image.h"


struct Node{
    std::pair<int, int> id; //depth, x
    std::string name;
    //вот это, возможно, плохо, а может и нет
    std::vector<std::shared_ptr<Mesh>> nodeMeshes = {};
};


class Model{
public:

    std::map<std::string, Texture> textureMap;

    std::vector<Texture> GetTextureMaterialPositions(aiMaterial* material, TextureType type, int MeshID); // УБРАТЬ ЛАСТ ПАРАМЕТР, НЕ НУЖЕН
    uint LoadTextureByPath(std::string path);

    bool NotOld(std::string ident);
    std::string directory;
    bool MateralExist;
   
    Assimp::Importer importer;
    const aiScene* scene;

    std::vector<std::shared_ptr<Mesh>> meshesPtr;

    std::vector<Node> nodes;


    Model(std::string modelName);

    void Draw(Shader & shader);
    void BadDump();
    void BadDump(int a);

    std::shared_ptr<Mesh> CreateMeshWithData(int meshID);

    void AddMesh(std::shared_ptr<Mesh> mesh);
    void ProccessNode(aiNode* node, int depth, int i);
};