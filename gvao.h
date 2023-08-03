#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "format.h"

class VBO 
{
    public: 
        VBO();

        ~VBO();
        //---
        void bind();
        void unbind();

        void LoadData(size_t size, void* data, format format);
    
        uint getId();
        
        // potentially dangerous
        format & getFormat();
    
        int getNumVertexes();
       

    private:
        uint vboid;
        size_t _size;    
        format _format; 
        uint numVertexes;
        uint numFloats;   

        void genId(); 
        
};

class EBO
{
    public:
        EBO();
       
        ~EBO();

        void bind();
        void unbind(); 
        
        
        void LoadData(size_t size, void* data);
        
        int getNumIndices() ;
        

    private:
        uint eboid; // this make sense..
        uint numIndices;

        uint getId() 
        {
            return eboid;
        }

        void genId() {
            glGenBuffers(1, &eboid);
        }
};

class VAO {

    public:
        VAO();
        ~VAO();
        //---
        void bind();
        void unbind();

        void bindToThis(VBO* vbo);
        void unbindFromThis(VBO* vbo);
   
        void bindToThis(EBO* ebo);
        void unbindFromThis(EBO* ebo);
        void pushVBO(VBO* vbo);
        void popVBO (VBO* vbo);
        //---

        void draw();
        void drawIndices();

        uint getId();

        int linkVBOSetAttrib(VBO* vbo, std::map<attribute, uint> attributes);

    private:
        uint vaoid;

        void genId();

        std::vector<VBO*> bindedVBOs;
        EBO* connectedEBO; 
}; 

