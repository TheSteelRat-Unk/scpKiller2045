#pragma once
#include <list>

enum attribute
{
    verticeAttr,
    normalAttr,
    texcoordsAttr,
    tangentAttr,
    bitangentAttr,
};

struct format 
{
    size_t size;
    std::list<std::pair<attribute, size_t>> datasizes;
};