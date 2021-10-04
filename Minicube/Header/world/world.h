#pragma once

#include <map>
#include <cmath>

#include "Glm/vec2.hpp"

#include "chunkMap.h"
#include "chunk.h"


namespace Minicube
{

    class World
    {
    public:
        World();
        void draw(const Shader &shader);
        Chunk *getChunk(const glm::ivec2 &pos);
        Block *getBlock(const glm::ivec3 &pos);

    private:
        ChunkMap m_chunks;
    };
} // namespace Minicube
