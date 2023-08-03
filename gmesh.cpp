#include "gmesh.h"
#include "format.h"

template<typename Vertex>
void Mesh<Vertex>::vertexesToByteData()
{
    vertexesByte = new unsigned char[vertexes.size() * vertexesFormat.size];
    unsigned char * Ptr   = vertexesByte;

    for(size_t i = 0; i < vertexes.size(); i++)
        vertexes[i].CopyThis(Ptr + i * vertexesFormat.size);
}

template<typename Vertex>
void Mesh<Vertex>::freeVertexesByteData()
{
    delete[] vertexesByte;
}

template<typename Vertex>
Mesh<Vertex>::~Mesh()
{
    std::cout << " There should be some vao + etc destructors " << std::endl;
}

template<typename Vertex>
Mesh<Vertex>::Mesh(const std::vector<Vertex> _vertexes, const std::vector<uint> _indices) :  
                                    vertexes(_vertexes), indices(_indices)
{ 
    std::cout << "constructor " << std::endl;
    vertexesFormat = vertexes[0].GetFormat(); // no comments..
}

template<typename Vertex>
VAO Mesh<Vertex>::getVAO()
{
    return vao;
}

template<typename Vertex>
void Mesh<Vertex>::LoadOutGPU(std::map<attribute, uint> needShader)
{
    format f = vertexes[0].GetFormat(); // лол

    ebo.bind();
    ebo.LoadData(sizeof(uint) * indices.size(), (void*)(indices.data()) );
    ebo.unbind();

    vao.bind();
    vao.bindToThis(&vbo);

    vertexesToByteData();
    void* dataPtr = static_cast<void*>(vertexesByte);
    vbo.LoadData(f.size * vertexes.size(), dataPtr, f);
    freeVertexesByteData();

    vao.linkVBOSetAttrib(&vbo, needShader);
    vao.bindToThis(&ebo);
    //ebo.unbind();
    //vbo.unbind();
    vao.unbind();
}
