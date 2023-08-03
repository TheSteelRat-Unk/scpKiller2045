#include "Graphics.h"
#include "Observer.h"


// clang++ -std=c++17 -stdlib=libc++ -lglfw -framework CoreVideo -framework OpenGL -framework IOKit -framework Cocoa -framework Carbon -lassimp -I ./include -I ./glm-0.9.9.7 gbaseVertexes.cpp VAO.cpp  shader_class.cpp gTextures.cpp WindowHandler.cpp gCamera.cpp gModel.cpp Loader.cpp  Graphics.cpp Observer.cpp  glad.c stbi.cpp
// 
using ObjPtr = std::shared_ptr<RenderObject>;
using ModPtr = std::shared_ptr<Model>;
//
Observer::Observer () { }
Observer::~Observer() { }

void Observer::RegisterObserver(Graphics* _graphics, GameScene* _gamescene) 
{
    graphics   = _graphics;
    graphics -> AddObserver(this);

    gamescene  =  _gamescene;
    CurrentLocation  = gamescene -> GetNext();
    LocationPosition = glm::vec3(0.0f);
}

void Observer::LoadLocToRender()
{  
    std::vector<ObjPtr> rooms;
    glm::vec3 position;
    for(auto & room : CurrentLocation.rooms)
    {
        position = LocationPosition + room.offsetInLocation;
        Motion roomMotion(position); // check this

        ModPtr roomModel = graphics -> GetModelById(room.modelKey);
        ObjPtr roomObj   = graphics -> CreateRenderObject(roomModel, roomMotion);
        rooms.push_back(roomObj);
    }
    auto & RenderList = graphics -> RenderList;
    RenderList.insert(RenderList.end(), rooms.begin(), rooms.end());

    //LocationPosition += CurrentLocation.endPoint;
}

void Observer::ObsUpdate()
{
    PlayerPos = graphics -> camera -> GetCameraPosition();
   
    std::cout << " z coord " << std::round(PlayerPos.z * 100)/100 << " border begin z coord " << CurrentLocation.borderzone_f[3].z + LocationPosition.z << std::endl;
    if(CheckInBorderZone())
    {
        CurrentLocation = gamescene -> GetNext();
        std::cout << " got location " << CurrentLocation.rooms.size() << std::endl;

        LocationPosition += CurrentLocation.endPoint;// +++

        LoadLocToRender();
    }
}

bool Observer::CheckInBorderZone()
{
    if(PlayerPos.x >= CurrentLocation.borderzone_f[0].x && 
       PlayerPos.x <= CurrentLocation.borderzone_f[1].x &&
       PlayerPos.z <= CurrentLocation.borderzone_f[3].z + LocationPosition.z && 
       PlayerPos.z >= CurrentLocation.borderzone_f[1].z + LocationPosition.z)
    {
        std::cout << " на границе перехода " << std::endl;
        return true;
    } else {
        return false;
    }
} //0 1/
  //2 3/

//---

GameScene::GameScene()  { current = 0; }
GameScene::~GameScene() { }

void GameScene::InitLocations(const std::vector<Location> & _locations)
{
    locations = _locations;
}

Location GameScene::GetNext()
{
    Location next = locations[current];
    current++;
    return next;
}

//===

int main()
{
    Graphics G;

    GameScene game;
    Room room1 {"1", glm::vec3(0.0f, 0.0f, -0.5f)}; 
    Room room2 {"1", glm::vec3(0.0f, 0.0f, -1.5f)};
    Room room3 {"1", glm::vec3(0.0f, 0.0f, -2.5f)};

    Location loc = { };
    loc.name = "start loc";
    loc.rooms = {room1, room2, room3};

    loc.entryPoint = glm::vec3(0.0f, 0.0f, 0.0f);
    loc.endPoint   = glm::vec3(0.0f, 0.0f, -3.0f);

    loc.borderzone_f = {
        glm::vec3(-0.5f, 0.0f, -3.0f), glm::vec3(0.5f, 0.0f, -3.0f),    
        glm::vec3(-0.5f, 0.0f, -2.0f), glm::vec3(-0.5f, 0.0f, -2.0f)}; //-2.5

    loc.borderzone_b = {
        glm::vec3(-1.0f, 0.0f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.5f),    
        glm::vec3(-1.5f, 0.0f, -0.5f), glm::vec3(-1.5f, 0.0f, 0.5f)};


    Observer observer;
    game.InitLocations(std::vector<Location>{loc, loc, loc, loc, loc, loc, loc});
    observer.RegisterObserver(&G, &game);

    //---------

    G.Init();
    G.GenerateMeshesForTest(); //
 

    Texture someTexture("road.jpg");
    //Texture secondTexture("wood.png");

    G.shader -> SetInt("texture1", 0); 
   

    std::vector<std::shared_ptr<MeshInterface>> meshes = {G.meshes[0], G.meshes[1]};
    G.TempLoadModel("1", meshes);
 
    Motion motion(glm::vec3(0.0f, 0.0f, -2.0f));
    auto obj = G.CreateRenderObject("1", motion);

    std::shared_ptr<RenderObject> objSh(obj);

    std::cout << " meshes num " << objSh -> GetRelatedModel() -> getMeshesPtr().size() << std::endl;

    //=========================
    float pastTime = glfwGetTime();
    float nowTime;


    observer.LoadLocToRender();

    //***
    //G.LoadModel("smth", "cottage.obj");
    //Motion modelMot(glm::vec3(-3.0f, 0.0f, -1.5f));
    //ObjPtr MODELOBJ =  G.CreateRenderObject("smth", modelMot);
    //***
    
    while (!glfwWindowShouldClose(G.window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // time
        nowTime  = glfwGetTime();
        float deltaTime = nowTime - pastTime;
        pastTime = nowTime;
        // camera
        G.camera -> AdjustWithTime(deltaTime);
        processInput(G.window, *G.camera);

        G.shader -> use();

        glm::mat4 view = G.camera -> GetView();
        G.shader -> SetMat4("view", view);
        
        glActiveTexture(GL_TEXTURE0);
        someTexture.Bind();

        observer.ObsUpdate();
        // main
        G.Render(*G.shader);

        //G.DrawRenderObject(MODELOBJ, *G.shader);
        

        glfwSwapBuffers(G.window);
        glfwPollEvents();
    }

    glfwTerminate();
    std::cout << " okey " << std::endl; 

    return 0;
}