#pragma once

#include <vector>

#include "chunk.h"
#include "world/gen/worldGen.h"

namespace Minicube
{
    class VerticalChunk
    {
    public:
        VerticalChunk(const glm::ivec2 &pos, ChunkMap *chunks);
        ~VerticalChunk()
        {
            for (auto it = m_chunks.begin(); it != m_chunks.end(); it++)
                delete *it;
            if (m_heightMap != nullptr)
                delete m_heightMap;
        }
        void generate(PerlinNoiseContext *perlinNoiseContext);

        inline const glm::ivec2 &getPos() const
        {
            return m_pos;
        }

        Chunk *getChunk(unsigned int index)
        {
            if (index >= m_chunks.size())
                return nullptr;
            return m_chunks[index];
        }

        inline unsigned int size()
        {
            return m_chunks.size();
        }

        inline std::vector<Chunk *>::iterator begin()
        {
            return m_chunks.begin();
        }

        inline std::vector<Chunk *>::iterator end()
        {
            return m_chunks.end();
        }

        inline std::vector<Chunk *>::iterator erase(std::vector<Chunk *>::iterator it)
        {
            delete *it;
            return m_chunks.erase(it);
        }

    private:
        glm::ivec2 m_pos;
        std::vector<Chunk *> m_chunks;
        ChunkMap *m_chunksMap;
        WorldGen::HeightMap *m_heightMap = nullptr;
    };
} // namespace Minicube
