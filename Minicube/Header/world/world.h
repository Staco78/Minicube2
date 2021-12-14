#pragma once

#include <map>
#include <cmath>
#include <thread>

#include "Glm/vec2.hpp"

#include "chunkMap.h"
#include "chunk.h"
#include "camera.h"
#include "utils/multiThreadQueue.h"

namespace Minicube
{

    class World
    {
    public:
        World(Camera *camera);
        void updateVisibleChunks();
        void draw(const Shader &shader);
        void startThreads();
        Chunk *getChunk(const glm::ivec3 &pos);
        Block *getBlock(const glm::ivec3 &pos);

    private:
        ChunkMap m_chunks;
        MultiThreadQueue<Chunk *> m_chunksToBuild;
        Camera *m_camera = nullptr;
        void updateChunksThread();
        int m_renderDistance = 12;
    };
} // namespace Minicube
