#pragma once
#include <iostream>
#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Graphics;

//---
struct Room 
{
    std::string modelKey;
    glm::vec3   offsetInLocation;
};

struct Location
{
    std::string         name;
    std::vector<Room>   rooms;
    
    // по дефолту лучше x = 0, y = 0 
    glm::vec3 entryPoint;
    glm::vec3 endPoint;

    // 0 -----  1 //
    // 2 ------ 3 //
    std::array<glm::vec3, 4> borderzone_f;
    std::array<glm::vec3, 4> borderzone_b;
};

class GameScene
{
    public:
        GameScene ();
        ~GameScene();

        Location GetNext();
        void InitLocations(const std::vector<Location> & _locations);

    private:
        std::vector<Location> locations;
        uint current;
};

//---
// локации
class Observer
{
    public:
        Observer ();
        ~Observer();

        void RegisterObserver(Graphics* _graphics, GameScene* _gamescene);
        void ObsUpdate();

    //move to private, temp here
        void LoadLocToRender();
        bool CheckInBorderZone();
    private:
        glm::vec3 PlayerPos;

        Location    CurrentLocation;
        glm::vec3   LocationPosition;

        Graphics*    graphics;
        GameScene*   gamescene;
};

/*
Графика  ->         обс1
         -> [инфа]  обс2  <-   СЦЕНА
         ->         обс3
         */