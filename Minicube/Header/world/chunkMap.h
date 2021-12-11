#pragma once

#include <map>
#include <iostream>

#include "Glm/vec3.hpp"

namespace std
{
    template <>
    struct less<glm::ivec3>
    {
        bool operator()(const glm::ivec3 &left, const glm::ivec3 &right) const
        {
            if (left.y == right.y && left.x == right.x)
                return left.z < right.z;
            else if (left.x == right.x)
                return left.y < right.y;
            else
                return left.x < right.x;
        }
    };
}

namespace Minicube
{

    class Chunk;

    class ChunkMap : public std::map<glm::ivec3, Chunk *>
    {
    public:
        Chunk *get(glm::ivec3 key)
        {
            auto it = find(key);
            if (it == end())
                return nullptr;
            return it->second;
        }

        ~ChunkMap()
        {
            for (auto it = this->begin(); it != this->end(); it++)
                delete it->second;
        }

        // std::map<glm::ivec2, Minicube::Chunk *>::iterator remove(std::map<glm::ivec2, Minicube::Chunk *>::iterator &it)
        // {
        //     delete it->second;
        //     return this->erase(it);
        // }
    };
}