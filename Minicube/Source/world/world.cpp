#include "world/world.h"

namespace Minicube
{
    World::World(Camera *camera)
    {
        m_camera = camera;
    }

    void World::draw(const Shader &shader)
    {
        int renderDistance = 3;
        glm::ivec2 playerChunkPos = glm::ivec2(floor(m_camera->getPosition().x / 16.0), floor(m_camera->getPosition().z / 16.0));
        Chunk *playerChunk = getChunk(playerChunkPos);
        if (playerChunk == nullptr)
        {
            m_chunks[playerChunkPos] = new Chunk(&m_chunks, playerChunkPos);
            playerChunk = getChunk(playerChunkPos);
        }
        for (int x = playerChunk->getPos().x - renderDistance; x <= playerChunk->getPos().x + renderDistance; x++)
            for (int y = playerChunk->getPos().y - renderDistance; y <= playerChunk->getPos().y + renderDistance; y++)
            {
                Chunk *chunk = getChunk(glm::ivec2(x, y));
                if (chunk == nullptr)
                    m_chunks[glm::ivec2(x, y)] = new Chunk(&m_chunks, glm::ivec2(x, y));
            }

        auto it = m_chunks.begin();
        while (it != m_chunks.end())
        {
            if (it->second->getPos().x > playerChunk->getPos().x - renderDistance &&
                it->second->getPos().x < playerChunk->getPos().x + renderDistance &&
                it->second->getPos().y > playerChunk->getPos().y - renderDistance &&
                it->second->getPos().y < playerChunk->getPos().y + renderDistance)
                
                it->second->draw(shader);
            else
            {
                it = m_chunks.remove(it);
            }
            it++;
        }
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
