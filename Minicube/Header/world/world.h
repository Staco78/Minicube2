#pragma once

#include <map>
#include <cmath>

#include "Glm/vec2.hpp"

#include "chunkMap.h"
#include "chunk.h"
#include "camera.h"


namespace Minicube
{

    class World
    {
    public:
        World(Camera* camera);
        void updateVisibleChunks();
        void draw(const Shader &shader);
        Chunk *getChunk(const glm::ivec3 &pos);
        Block *getBlock(const glm::ivec3 &pos);

    private:
        ChunkMap m_chunks;
        Camera* m_camera = nullptr;
        int m_renderDistance = 12;
    };
} // namespace Minicube
