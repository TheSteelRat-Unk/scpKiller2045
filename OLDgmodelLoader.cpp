#include "model.h"


void Model::Draw(Shader & shader){
    for(auto & M : meshesPtr){
        M->Draw(shader);
    }
}

std::vector<Texture> Model::GetTextureMaterialPositions(aiMaterial* material, TextureType type, int MeshID){

    std::cout << " загрузка для материала для меша id : " << MeshID << std::endl;
    aiString pathAiStr;
    std::string path;

    std::vector<Texture> matTexs = {};
    Texture current = {};

    int mapSizeBefore = textureMap.size();
    if(MateralExist){
        for(unsigned i = 0; i < material->GetTextureCount(ConvertToAssimpTType(type)); i++){

            std::cout << " количество текстур типа в материале меша" << material->GetTextureCount(ConvertToAssimpTType(type)) << std::endl;


            if(material->GetTexture(ConvertToAssimpTType(type), i, &pathAiStr) == AI_SUCCESS){
                path = pathAiStr.C_Str(); 
                
                current.path = path;
                current.type = type;
                std::string ident = path + ConvertToStringTType(type);
                
                if(NotOld(ident)){
                    uint texGLid = LoadTextureByPath(path); 
                    current.texID = texGLid;                

                    textureMap[ident] = current;
                } else {
                    current = textureMap[ident];        //ничего не делаем с мап, просто генерим старое оттуда
                }
                
                matTexs.push_back(current);
    
            }
        }

    }
    int mapSizeAfter = textureMap.size(); 
    std::cout << " map change " << mapSizeAfter - mapSizeBefore << std::endl;

    return matTexs;
}

bool Model::NotOld(std::string ident){
    std::cout << "check " << ident << std::endl;
    if(textureMap.find(ident) == textureMap.end()) {  

        return true; 
    }
    return false;
}

uint Model::LoadTextureByPath(std::string path){
    
    std::cout << " - Работа над загрузкой из файла... - " << std::endl;
    std::string filename = path;
    std::cout << " filename " << filename << std::endl;

  
    uint texID;
    glGenTextures(1, &texID);


    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {   
        std::cout << " success data load " << std::endl;

        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;


        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else {
         std::cout << "Failed to load texture" << std::endl; 
         return 0;
    }
    stbi_image_free(data);
    std::cout << " - success -" << std::endl;
    return texID;
}

void Model::BadDump(){
    for(auto & elem : nodes){
        std::cout << "node number..." << elem.id.first << " " << elem.id.second << std::endl;
        for(auto & msh : elem.nodeMeshes){
            msh->Dump();
        }
    }

}

// Refactor
void Model::BadDump(int a){
    std::cout << " +++++ ";
    for(auto & elem : nodes){
        std::cout << " ID pair " << elem.id.first << " " << elem.id.second << std::endl;
        std::cout << "it's meshes NUM" << elem.nodeMeshes.size() << std::endl;

        for( auto & mesh : elem.nodeMeshes)
        { 
           mesh->Dump();
        }
    }
}

Model::Model(std::string modelName){
    scene = importer.ReadFile(modelName.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
         std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
         return;
    }

    directory = modelName.substr(0, modelName.find_last_of('/'));
    MateralExist = scene->HasMaterials();
}

void Model::AddMesh(std::shared_ptr<Mesh> mesh){
    meshesPtr.push_back(std::move(mesh));
}

std::shared_ptr<Mesh> Model::CreateMeshWithData(int meshID){

    std::vector<VertexData> vData;
    std::vector<int> indices;
    std::vector<Texture> textures; 

    aiMesh* mesh = scene->mMeshes[meshID];
    
    for (int i = 0; i < mesh->mNumVertices; i++) {
        vData.emplace_back(VertexData {

            {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z },

            mesh->HasNormals() ? 
            glm::vec3{ mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z } :           glm::vec3{},

            mesh->mTextureCoords[0] != nullptr ? 
            glm::vec2 { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y } :             glm::vec3{},

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

    uint matID = mesh->mMaterialIndex;
    aiMaterial* material = scene->mMaterials[matID];  


    for(TextureType value = DIFFUSE; value <= UNKNOWN; value = static_cast<TextureType>
    (static_cast<int>(value) + 1)) {
        std::cout << " processing " << ConvertToStringTType(value);
        std::vector<Texture> typeTextures= GetTextureMaterialPositions(material, value, meshID);
        textures.insert(textures.end(), typeTextures.begin(), typeTextures.end());
    }

    std::cout << "map size" << textureMap.size() << "***" << std::endl;

    Mesh* meshRet = new Mesh(vData, textures, indices);
    return std::shared_ptr<Mesh>(meshRet);
}

void Model::ProccessNode(aiNode* node, int depth, int i){

    int numChildren = node->mNumChildren; 
    int meshNumber  = node->mNumMeshes;

    std::cout << "=======================" << std::endl;
    std::cout << " Name " << node->mName.C_Str() << " Num Child: " << numChildren << std::endl; //Debug
    std::cout << " Depth " << depth << " X coord " << i << std::endl; //Debug
    // gen model class node
    std::pair <int, int> nodeKey(depth, i);

    Node currentNode = {};
    currentNode.id = nodeKey;
    currentNode.name = "fix it"; 

    std::cout << " Meshes to process: " << meshNumber << std::endl;
    for(unsigned int j = 0; j < meshNumber; j++) {
        // get mesh-assimp real ID and load Attrib (mesh class process it)
        int meshID = node->mMeshes[j];
        // normas
        std::shared_ptr<Mesh> mesh = CreateMeshWithData(meshID); 
        // add mesh class obj pointer to vector + connect with correct node
        AddMesh(mesh);
        currentNode.nodeMeshes.push_back(std::move(mesh));
    }
    nodes.push_back(currentNode);

    for(unsigned int i = 0; i < numChildren; i++) {
        ProccessNode(node->mChildren[i], depth + 1, i);
    }
}