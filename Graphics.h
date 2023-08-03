#pragma once
class Observer;

#include <cmath>
#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <algorithm>
#include <functional>

#include "shader_class.h"
#include "gCamera.h"
#include "WindowHandler.h"
#include "stb_image.h"
//#include "gTextures.h"


#include "Loader.h"

void processInput(GLFWwindow *window, Camera & camera);

class Graphics
{
    public:
        Graphics();
        ~Graphics() { }

        void Init();

        Shader*  shader; // private
        Camera*  camera; 
        Loader*  loader; 
    

        std::vector<std::shared_ptr<MeshInterface>> meshes;

        void CreateWindow(int width, int height, std::string title); 
        GLFWwindow* window;  

        void Render(Shader shader); 
        //Temp. Generates 2 base meshes 
        void GenerateMeshesForTest();

        void           TempLoadModel   (const std::string modelId, std::vector<std::shared_ptr<MeshInterface>> & meshesPtr);

        void LoadModel(const std::string modelId, const std::string path); 

        std::shared_ptr<RenderObject>  CreateRenderObject(const std::shared_ptr<Model> modelPtr, const Motion & motion);
        std::shared_ptr<RenderObject>  CreateRenderObject(const std::string modelId, const Motion & motion);
        std::shared_ptr<Model>         GetModelById (std::string id);

        void          DrawRenderObject (std::shared_ptr<RenderObject> renderObj, Shader & shader);
    
        void AddObserver  (Observer* _observer);
        Observer* observer;

    private:
        friend class Observer; // хзхз

        std::vector<std::shared_ptr<Model>>            Models;
        std::vector<std::shared_ptr<RenderObject>> RenderList;

        std::list<Texture> textureCollection;

        WindowHandler windowHandler;

};

