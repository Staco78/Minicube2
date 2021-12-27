#pragma once

#include <map>
#include <cmath>
#include <thread>
#include <algorithm>

#include "Glm/vec2.hpp"

#include "chunkMap.h"
#include "chunk.h"
#include "camera.h"
#include "gen/worldGen.h"
#include "heightMaps.h"

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
        ChunkMap *getChunkMap()
        {
            return &m_chunks;
        }

        WorldGen::HeightMapData *getHeightMapData(const glm::ivec2 &pos)
        {
            Chunk *chunk = getChunk(glm::ivec3(pos.x / 16, 0, pos.y / 16));
            if (chunk == nullptr)
                return nullptr;

            WorldGen::HeightMap *heightMap = chunk->getHeightMap();
            if (heightMap == nullptr)
                return nullptr;

            glm::uvec3 blockOffset = utils::getBlockOffset(pos.x, 0, pos.y);
            return &heightMap->data[blockOffset.x * 16 + blockOffset.z];
        }

    private:
        ChunkMap m_chunks;
        HeightMaps m_heightMaps;
        Camera *m_camera = nullptr;
        void updateChunksThread();
        void generateChunksThread();
        int m_renderDistance = 12;

        PerlinNoiseContext m_perlinNoiseContext;
    };
} // namespace Minicube
