#pragma once

#include <map>
#include <iostream>

#include "Glm/vec2.hpp"

namespace std
{
    template <>
    struct less<glm::ivec2>
    {
        bool operator()(const glm::ivec2 &left, const glm::ivec2 &right) const
        {
            if (left.x == right.x)
                return left.y < right.y;
            else
                return left.x < right.x;
        }
    };
}

namespace Minicube
{

    class Chunk;

    class ChunkMap : public std::map<glm::ivec2, Chunk *>
    {
    public:
        Chunk *get(glm::ivec2 key)
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
    };
}