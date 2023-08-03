#include "gvao.h"
//*********************************************************************************************
//---
// Utils.h
// Logger в отдельный файл + быстрое создание форматов 



// подумать про класс Manager, который будет принимать инфу от VBO о том, что совершен "просто бинд, без привязки"
// И инфу от VAO касательно его бинда, тупо uint id
// И класс VAO будет искать такой неправильный бинд и принудительно добавлять к себе в вектор 


VBO::VBO()
{
    std::cout << " vbo constr " << std::endl;
    numVertexes = 0; // if bind empty = million triangles
    genId();
}

VBO::~VBO()
{ }

void VBO::bind() 
{ 
    glBindBuffer(GL_ARRAY_BUFFER, vboid); 
}

void VBO::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::LoadData(size_t size, void* data, format format) 
{
    _size = size;
    _format = format;
    std::cout << "=====debug vbo=====" << std::endl;
    float* floatData = static_cast<float*>(data); 
    for (size_t i = 0; i < size / sizeof(float); i++) {
        printf("%f ", floatData[i]); 
    }
    printf("\n");
    std::cout << "=================== " << std::endl;

    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW); 

    uint numVerticesPerVertex = static_cast<uint>(format.size/sizeof(float));
    numVertexes          = static_cast<uint>(size/format.size);

    numFloats              = numVertexes * numVerticesPerVertex;
}

uint VBO::getId() 
{
    return vboid;
}

format & VBO::getFormat() 
{  
    return _format;
}

int VBO::getNumVertexes()
{
    std::cout << " данные по количеству вершин из VBO " << numVertexes << std::endl;
    return numVertexes;
}

void VBO::genId() 
{
    glGenBuffers(1, &vboid);
}


//********************

EBO::EBO()
{
    std::cout << " ebo constr " << std::endl;
    genId();
}
        EBO::~EBO() {}

        void EBO::bind() 
        { 
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboid); 
        }
        void EBO::unbind() 
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }
        //---
        void EBO::LoadData(size_t size, void* data) 
        {
            std::cout << "=====debug ebo=====" << std::endl;
            uint* uintData = static_cast<uint*>(data); 
            for (size_t i = 0; i < size / sizeof(uint); i++) {
                 printf("%d ", uintData[i]); 
            }
            std::cout << std::endl << "===================" << std::endl;
    
            numIndices = static_cast<uint>(size/sizeof(float));
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        }
        int EBO::getNumIndices() 
        {
            return numIndices;
        }



//***************
VAO::VAO() 
{
    std::cout << " vao constr " << std::endl;
    genId();
}
VAO::~VAO() {}
        //---
void VAO::bind()
{
    glBindVertexArray(vaoid);
}
void VAO::unbind()
{
    glBindVertexArray(0);
}

void VAO::bindToThis(VBO* vbo) 
{
    vbo -> bind();
    this -> pushVBO(vbo);
}

void VAO::unbindFromThis(VBO* vbo) 
{
    vbo -> unbind();
    this -> popVBO(vbo);
}
        //---
void VAO::bindToThis(EBO* ebo) 
{
    ebo -> bind();
    this -> connectedEBO = ebo;
}

void VAO::unbindFromThis(EBO* ebo) 
{
    ebo -> unbind();
    this -> connectedEBO = ebo;
}

uint VAO::getId()
{
    return vaoid;
}

void VAO::genId()
{
    glGenVertexArrays(1, &vaoid);
}




void VAO::pushVBO(VBO* vbo) 
{
    bindedVBOs.push_back(vbo);
}

void VAO::popVBO(VBO* vbo) 
{
    auto it = std::find(bindedVBOs.begin(), bindedVBOs.end(), vbo);
    if(it != bindedVBOs.end())
    {
        bindedVBOs.erase(it);
    }
    else 
        std::cout << " already unbinded ! make logger " << std::endl;
}

void VAO::draw() 
{
    uint totalVertexes = 0; 
    std::cout << "total " << totalVertexes << " ) " << std::endl;//
    for(auto & currVbo : bindedVBOs)
    {
        totalVertexes += currVbo->getNumVertexes(); 
    }                                               // там ж мусор по дефолту. Проверять над
    std::cout << "total " << totalVertexes << " ) " << std::endl;
    glDrawArrays(GL_TRIANGLES, 0, totalVertexes);
}

void VAO::drawIndices()
{
    if(connectedEBO)
    {
        //std::cout << " количество индексов с EBO " << connectedEBO -> getNumIndices() << std::endl;
        glDrawElements(GL_TRIANGLES, connectedEBO -> getNumIndices(), GL_UNSIGNED_INT, 0);
    }
    else
        std::cout << "А он исчез " << std::endl;
}

int VAO::linkVBOSetAttrib(VBO* vbo, std::map<attribute, uint> attributes) 
{
    std::list<std::pair<attribute, size_t>> vbosData = vbo->getFormat().datasizes;

    size_t offset   = 0;
    size_t stride   = vbo->getFormat().size;

    for(auto it = vbosData.begin(); it != vbosData.end(); it++)
    {
        attribute vbosAttrib = it -> first;
        size_t    currSize   = it -> second;

        if(attributes.find(vbosAttrib) != attributes.end())
        {
            uint layout = attributes[vbosAttrib];
            int num     = static_cast<GLint>(currSize/sizeof(float));

            std::cout << " layout " << (GLuint)layout << " num " << num << " stride " << (GLsizei)stride << " offset " << (GLvoid*)offset << std::endl;
            glVertexAttribPointer((GLuint)layout, num, GL_FLOAT, GL_FALSE, GLsizei(stride), (GLvoid*)offset);
            glEnableVertexAttribArray((GLuint)layout);  
        }

        offset = offset + currSize;
    }
}
