#include "world/verticalChunk.h"

namespace Minicube
{
    VerticalChunk::VerticalChunk(const glm::ivec2 &pos, ChunkMap *chunks)
    {
        m_pos = pos;
        m_chunksMap = chunks;
    }

    void VerticalChunk::generate(PerlinNoiseContext *perlinNoiseContext)
    {
        m_heightMap = new WorldGen::HeightMap(m_pos.x, m_pos.y, perlinNoiseContext);
        int nbChunks = m_heightMap->maxHeight / 16 + 1;
        for (int y = 0; y < nbChunks; y++)
        {
            glm::ivec3 pos(m_pos.x, y, m_pos.y);
            Chunk *chunk = new Chunk(m_chunksMap, pos);
            chunk->generate(m_heightMap);
            m_chunks.push_back(chunk);
        }
    }
} // namespace Minicube
