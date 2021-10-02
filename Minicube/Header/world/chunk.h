#pragma once

#include <map>

#include "Glm/vec3.hpp"

#include "blocks/block.h"

namespace std
{
    template <>
    struct less<glm::uvec3>
    {
        bool operator()(const glm::uvec3 &left, const glm::uvec3 &right) const
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
    class BlockMap : public std::map<glm::uvec3, Block *>
    {
    public:
        Block *get(glm::ivec3 pos)
        {

            if (pos.x < 0 || pos.y < 0 || pos.z < 0 || pos.x > 15 || pos.z > 15)
                return nullptr;

            auto it = find(pos);
            if (it == end())
                return nullptr;
            return it->second;
        }

        ~BlockMap()
        {
            for (auto it = this->begin(); it != this->end(); it++)
                delete it->second;
        }
    };

    class Chunk
    {
    public:
        Chunk(glm::ivec2 pos);
        void draw(const Shader &shader);
        void addBlock(glm::uvec3 relativeBlockPos);
    private:
        BlockMap m_blocks;
        glm::ivec2 m_pos;
    };

} // namespace Minicube
