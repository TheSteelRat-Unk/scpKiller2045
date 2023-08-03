#pragma once
#include "gModel.h"
#include "gTextures.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Loader
{
    public:
        Loader();
        //Loader(std::list<Texture> & _textureCollection);
        ~Loader();

        void LoadModel(std::string path);
        std::vector<std::shared_ptr<MeshInterface>> ReturnData();

    private:
        //
        //std::list<Texture> & textureCollection;

        //
        
        void ProcessNode(aiNode* node, int depth, int i);
        std::shared_ptr<MeshInterface> CreateMeshWithData(int meshID);

        std::vector<std::shared_ptr<MeshInterface>> meshes;

        Assimp::Importer importer;
        const aiScene* scene;

        bool MaterialExist;
};
