#include "world/world.h"

namespace Minicube
{
    World::World(Camera *camera)
    {
        m_camera = camera;
    }

    bool isVisible(const glm::ivec2 &chunkPos, const glm::ivec2 &playerChunkPos, int renderDistance)
    {
        return chunkPos.x >= playerChunkPos.x - renderDistance && chunkPos.x <= playerChunkPos.x + renderDistance &&
               chunkPos.y >= playerChunkPos.y - renderDistance && chunkPos.y <= playerChunkPos.y + renderDistance;
    }

    void World::updateVisibleChunks()
    {
        glm::ivec2 playerChunkPos = glm::ivec2(floor(m_camera->getPosition().x / 16.0), floor(m_camera->getPosition().z / 16.0));

        std::vector<Chunk *> free;

        auto it = m_chunks.begin();
        while (it != m_chunks.end())
        {
            if (!isVisible(it->second->getPos(), playerChunkPos, m_renderDistance))
            {
                it->second->~Chunk();
                free.push_back(it->second);
                it = m_chunks.erase(it);
            }
            else
                it++;
        }

        for (int x = playerChunkPos.x - m_renderDistance; x <= playerChunkPos.x + m_renderDistance; x++)
        {
            for (int y = playerChunkPos.y - m_renderDistance; y <= playerChunkPos.y + m_renderDistance; y++)
            {
                glm::ivec2 pos(x, y);
                if (getChunk(pos) == nullptr)
                {
                    if (free.size() == 0)
                    {
                        m_chunks[pos] = new Chunk(&m_chunks, pos);
                    }
                    else
                    {
                        Chunk *ptr = free.back();
                        free.pop_back();

                        void *place = ptr;
                        m_chunks[pos] = new (place) Chunk(&m_chunks, pos);
                    }
                    m_chunks[pos]->generate();
                }
            }
        }
    }

    void World::draw(const Shader &shader)
    {
        int VBOConstructed = 0;
        for (auto it = m_chunks.begin(); it != m_chunks.end(); it++)
            it->second->draw(shader, VBOConstructed);
    }

    Chunk *World::getChunk(const glm::ivec2 &pos)
    {
        return m_chunks.get(pos);
    }

    Block *World::getBlock(const glm::ivec3 &pos)
    {

        Chunk *chunk = m_chunks.get(glm::ivec2(floor(pos.x / 16.0), floor(pos.z / 16.0)));

        int x;
        int z;

        if (pos.x % 16 == 0)
            x = 0;
        else if (pos.x >= 0)
            x = pos.x % 16;
        else
            x = 16 + (pos.x % 16);

        if (pos.z % 16 == 0)
            z = 0;
        else if (pos.z >= 0)
            z = pos.z % 16;
        else
            z = 16 + (pos.z % 16);

        return chunk->getBlock(glm::uvec3(x, pos.y, z));
    }

} // namespace Minicube
