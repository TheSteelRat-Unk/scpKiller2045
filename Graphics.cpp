#include "Graphics.h"

Graphics::Graphics()
{
    CreateWindow(800, 600, "Aghm"); //1024 768
    glEnable(GL_DEPTH_TEST);
}

void Graphics::Init()
{
    camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), 0.2, 0.1, window); //2.5
    shader = new Shader("shaderVS.fs", "shaderFR.fs");
    loader = new Loader;

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1024 / (float)768, 0.1f, 100.0f);
    shader -> use();
    
    shader -> SetMat4("projection", projection);
}

// temp for test
void Graphics::LoadModel(const std::string modelId, const std::string path)
{
    loader -> LoadModel(path);
    std::vector<std::shared_ptr<MeshInterface>> modelMeshes = loader -> ReturnData();
    std::cout << " всего мешей " << modelMeshes.size() << std::endl;

    for(auto & mesh : modelMeshes)
    {
        mesh -> LoadOutGPU(std::map<attribute, uint> { {verticeAttr, 0}, {texcoordsAttr, 1} } );
    }

    std::shared_ptr<Model> model = std::make_shared<Model>(modelId, modelMeshes); //по идее ок
    Models.push_back(std::move(model));   
}

void Graphics::Render(Shader shader)
{
    if(RenderList.size() > 0)
    {
        for(auto & RenObj : RenderList)
        {
            DrawRenderObject(RenObj, shader);
        }
    } else {
        throw::std::runtime_error(" empty render list ");
    }
}

void Graphics::AddObserver(Observer* _observer)
{
    observer = _observer;
}

// remake
void Graphics::DrawRenderObject(std::shared_ptr<RenderObject> renderObj, Shader & shader)
{   
    glm::mat4 matrix = renderObj -> GetModelMatrix();
    shader.SetMat4("model", matrix);
    
    auto meshes = renderObj -> GetRelatedModel() -> getMeshesPtr(); // метод Draw() у мешей;
    for(auto & mesh : meshes)
    { 
        auto vao = mesh -> getVAO(); //это не здесь должно быть 
        vao.bind();
        vao.drawIndices();
        vao.unbind();
    }
}

void Graphics::TempLoadModel(const std::string modelId, std::vector<std::shared_ptr<MeshInterface>> & meshesPtr)
{
    std::shared_ptr<Model> model = std::make_shared<Model>(modelId, meshesPtr);
    Models.push_back(model);
}

std::shared_ptr<Model> Graphics::GetModelById(std::string id)
{ 
    auto it = std::find_if(Models.begin(), Models.end(), [&id](std::shared_ptr<Model> current ) {
        return current -> getModelId() == id;
    });
    if(it == Models.end()){
        throw::std::runtime_error(" no model with such id");
    } else {
        return *it;
    }
}


std::shared_ptr<RenderObject> Graphics::CreateRenderObject(const std::shared_ptr<Model> modelPtr, const Motion & motion)
{
    std::shared_ptr<RenderObject> newObj = std::make_shared<RenderObject>(modelPtr, motion);
    return newObj;
}

std::shared_ptr<RenderObject> Graphics::CreateRenderObject(const std::string modelId, const Motion & motion)
{
    std::shared_ptr<RenderObject> newObj = std::make_shared<RenderObject>(GetModelById(modelId), motion);
    return newObj;
}

void Graphics::CreateWindow(int width, int height, std::string title)
{
    try
    {
        bool success = windowHandler.CreateWindow(width, height, title);
        if(!success)
           throw::std::runtime_error("graphics, no window");
        window = windowHandler.GetWindow();
    }
    catch(const std::exception & err)
    {
        std::cout << " err " << err.what() << std::endl;        
    }
}

void Graphics::GenerateMeshesForTest()
{
    // left && right
    BaseVertex v1_0(glm::vec3(-0.5f, 0.5f,  -0.5f), glm::vec2(0.0f, 0.99f)); // 0
    BaseVertex v1_1(glm::vec3(-0.5f, 0.5f,  +0.5f), glm::vec2(0.99f, 0.99f)); // 1
    BaseVertex v1_2(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.99f, 0.0f)); //  2
    BaseVertex v1_3(glm::vec3(-0.5f, -0.5f, +0.5f), glm::vec2(0.0f, 0.0f)); //  3

    BaseVertex v1_4(glm::vec3(+0.5f, 0.5f,  -0.5f), glm::vec2(0.0f, 0.99f)); // 4
    BaseVertex v1_5(glm::vec3(+0.5f, 0.5f,  +0.5f), glm::vec2(0.99f, 0.99f)); // 5
    BaseVertex v1_6(glm::vec3(+0.5f, -0.5f, -0.5f), glm::vec2(0.99f, 0.0f)); //  6
    BaseVertex v1_7(glm::vec3(+0.5f, -0.5f, +0.5f), glm::vec2(0.0f, 0.0f)); //   7 

    std::vector<BaseVertex>vertexes1 = {v1_0, v1_1, v1_2, v1_3, v1_4, v1_5, v1_6, v1_7};

    std::vector<uint> indices1 = {
    0, 1, 3, 0, 3, 2, 
    4, 5, 7, 7, 4, 6}; 

    auto M1 = std::make_shared< Mesh<BaseVertex> > (vertexes1, indices1);
    meshes.push_back(M1);
   
    meshes[0] -> LoadOutGPU(std::map<attribute, uint> { {verticeAttr, 0}, {texcoordsAttr, 1} } );
    //
    // top and bottom
    BaseVertex v2_0(glm::vec3(-0.5f, 0.5f,  0.5f), glm::vec2(0.0f, 0.0f)); // 
    BaseVertex v2_1(glm::vec3(0.5f, 0.5f,  0.5f), glm::vec2(0.99f, 0.0f)); //  
    BaseVertex v2_2(glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(0.99f, 0.0f)); // 
    BaseVertex v2_3(glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f)); // 

    BaseVertex v2_4(glm::vec3(-0.5f, 0.5f,  -0.5f), glm::vec2(0.0f, 0.99f)); // 
    BaseVertex v2_5(glm::vec3(0.5f, 0.5f,  -0.5f), glm::vec2(0.99f, 0.99f)); //  
    BaseVertex v2_6(glm::vec3(0.5f, -0.5f,  -0.5f), glm::vec2(0.99f, 0.99)); // 
    BaseVertex v2_7(glm::vec3(-0.5f,  -0.5f,  -0.5f), glm::vec2(0.0f, 0.99f)); // 

    std::vector<BaseVertex>vertexes2 = {v2_0, v2_1, v2_2, v2_3, v2_4, v2_5, v2_6, v2_7};

    std::vector<uint> indices2 = {
    0, 1, 5, 5, 0, 4, //front 
    3, 2, 6, 6, 3, 7}; // right

    auto M2 = std::make_shared< Mesh<BaseVertex> > (vertexes2, indices2);
    meshes.push_back(M2);

    meshes[1] -> LoadOutGPU(std::map<attribute, uint> { {verticeAttr, 0}, {texcoordsAttr, 1} } );

    
    /*
    DefVertex v3_1(glm::vec3(-2.0f, -0.5f,  0.0f), glm::vec2(1.0f),
    glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f)); //
    DefVertex v3_2(glm::vec3(-1.0f, -0.5f,  0.0f), glm::vec2(1.0f),
    glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f)); // 1
    DefVertex v3_3(glm::vec3(-2.0f,  1.0f,  0.0f), glm::vec2(1.0f),
    glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f)); // 2 top //0.3 было х
    DefVertex v3_4(glm::vec3(-1.0f,  1.0f,  -0.5f), glm::vec2(1.0f),
    glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(7.0f));

    std::vector<DefVertex> vertexes3 = {v3_1, v3_2, v3_3, v3_4};
    std::vector<uint> indices3 = {0, 1, 2, 1, 2, 3};

    auto M3 = std::make_shared < Mesh <DefVertex> > (vertexes3, indices3);
    meshes.push_back(M3);

    meshes[2] -> LoadOutGPU(std::map<attribute, uint> { {verticeAttr, 0} , {bitangentAttr, 5}, {normalAttr, 4}, {texcoordsAttr, 1} });
    */
}
//
//
// Rework this piece of shit complitely

void processInput(GLFWwindow *window, Camera & camera)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
        
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    { 
        camera.UpdateForward();
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.UpdateBack();
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.UpdateLeft();
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.UpdateRight();
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        GLint polygonMode[2]; 
        glGetIntegerv(GL_POLYGON_MODE, polygonMode);

        if (polygonMode[0] == GL_FILL && polygonMode[1] == GL_FILL) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } 
        else if (polygonMode[0] == GL_LINE && polygonMode[1] == GL_LINE) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    }
}