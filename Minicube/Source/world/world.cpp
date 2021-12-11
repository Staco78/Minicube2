#include "world/world.h"

namespace Minicube
{
    World::World(Camera *camera)
    {
        m_camera = camera;
    }

    bool isVisible(const glm::ivec3 &chunkPos, const glm::ivec3 &playerChunkPos, int renderDistance)
    {
        return chunkPos.x >= playerChunkPos.x - renderDistance && chunkPos.x <= playerChunkPos.x + renderDistance &&
               chunkPos.z >= playerChunkPos.z - renderDistance && chunkPos.z <= playerChunkPos.z + renderDistance;
    }

    void World::updateVisibleChunks()
    {
        glm::ivec3 playerChunkPos = glm::ivec3(floor(m_camera->getPosition().x / 16.0), floor(m_camera->getPosition().y / 16.0), floor(m_camera->getPosition().z / 16.0));

        auto it = m_chunks.begin();
        while (it != m_chunks.end())
        {
            if (!isVisible(it->second->getPos(), playerChunkPos, m_renderDistance))
            {
                it->second->~Chunk();
                it = m_chunks.erase(it);
            }
            else
                it++;
        }

        for (int x = playerChunkPos.x - m_renderDistance; x <= playerChunkPos.x + m_renderDistance; x++)
        {
            for (int y = 0; y <= 5; y++)
            {
                for (int z = playerChunkPos.z - m_renderDistance; z <= playerChunkPos.z + m_renderDistance; z++)
                {
                    glm::ivec3 pos(x, y, z);
                    if (getChunk(pos) == nullptr)
                    {

                        m_chunks[pos] = new Chunk(&m_chunks, pos);
                        m_chunks[pos]->generate();
                    }
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

    Chunk *World::getChunk(const glm::ivec3 &pos)
    {
        return m_chunks.get(pos);
    }

    Block *World::getBlock(const glm::ivec3 &pos)
    {

        Chunk *chunk = m_chunks.get(glm::ivec3(floor(pos.x / 16.0), floor(pos.y / 16.0), floor(pos.z / 16.0)));

        int x;
        int y;
        int z;

        if (pos.x % 16 == 0)
            x = 0;
        else if (pos.x >= 0)
            x = pos.x % 16;
        else
            x = 16 + (pos.x % 16);

        if (pos.y % 16 == 0)
            y = 0;
        else if (pos.y >= 0)
            y = pos.y % 16;
        else
            y = 16 + (pos.y % 16);

        if (pos.z % 16 == 0)
            z = 0;
        else if (pos.z >= 0)
            z = pos.z % 16;
        else
            z = 16 + (pos.z % 16);

        return chunk->getBlock(x, y, z);
    }

} // namespace Minicube
