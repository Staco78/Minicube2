#pragma once

#include <map>
#include <cmath>
#include <thread>
#include <algorithm>

#include "Glm/vec2.hpp"

#include "chunkMap.h"
#include "chunk.h"
#include "camera.h"
#include "gen/heightMap.h"

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
        Block *getBlock(int x, int y, int z);

    private:
        ChunkMap m_chunks;
        Camera *m_camera = nullptr;
        void updateChunksThread();
        void generateChunksThread();
        int m_renderDistance = 12;

        PerlinNoiseContext m_perlinNoiseContext;
    };
} // namespace Minicube
