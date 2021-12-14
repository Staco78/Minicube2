#include "world/world.h"

namespace Minicube
{
    World::World(Camera *camera)
    {
        m_camera = camera;
    }

    inline bool isVisible(const glm::ivec3 &chunkPos, const glm::ivec3 &playerChunkPos, int renderDistance)
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
            for (int y = 0; y < 10; y++)
            {
                for (int z = playerChunkPos.z - m_renderDistance; z <= playerChunkPos.z + m_renderDistance; z++)
                {
                    glm::ivec3 pos(x, y, z);
                    if (getChunk(pos) == nullptr)
                    {
                        Chunk *chunk = new Chunk(&m_chunks, pos);
                        chunk->generate();
                        m_chunks.set(pos, chunk);
                        m_chunksToBuild.push(chunk);
                    }
                }
            }
        }
    }

    void World::draw(const Shader &shader)
    {
        // m_chunks.lock();
        for (auto it = m_chunks.begin(); it != m_chunks.end(); it++)
        {
            it->second->draw(shader);
        }
        // m_chunks.unlock();
    }

    void World::updateChunksThread()
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        while (true)
        {
            while (!m_chunksToBuild.empty())
            {
                // std::cout << "Popping" << std::endl;
                m_chunksToBuild.pop()->constructVBO();
            }
            // std::cout << "Sleepping" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void World::startThreads()
    {
        std::thread updateChunksThread(&World::updateChunksThread, this);
        updateChunksThread.detach();
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
