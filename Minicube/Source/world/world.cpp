#include "world/world.h"

namespace Minicube
{
    World::World(Camera *camera)
    {
        m_camera = camera;
    }

    inline bool isVisible(const glm::ivec3 &chunkPos, int playerChunkPosX, int playerChunkPosZ, int renderDistance)
    {
        return chunkPos.x >= playerChunkPosX - renderDistance && chunkPos.x <= playerChunkPosX + renderDistance &&
               chunkPos.z >= playerChunkPosZ - renderDistance && chunkPos.z <= playerChunkPosZ + renderDistance;
    }

    void World::updateVisibleChunks()
    {
        glm::vec3 camPos = m_camera->getPosition();
        int playerChunkPosX = int(camPos.x) / 16;
        int playerChunkPosZ = int(camPos.z) / 16;
        int playerChunkPosY = int(camPos.y) / 16;

        m_chunks.lock();
        auto it = m_chunks.begin();
        while (it != m_chunks.end())
        {
            if (!isVisible(it->first, playerChunkPosX, playerChunkPosZ, m_renderDistance))
            {
                int state = it->second->getState();
                if (state != 2)
                {
                    if (state == 1)
                        m_chunksToBuild.erase(it->second);
                    else
                        m_chunksToRender.erase(it->second);
                    it->second->~Chunk();
                    it = m_chunks.erase(it);
                }
            }
            else
            {
                ++it;
            }
        }
        m_chunks.unlock();

        for (int x = playerChunkPosX - m_renderDistance; x <= playerChunkPosX + m_renderDistance; x++)
        {
            for (int y = 0; y < 3; y++)
            {
                for (int z = playerChunkPosZ - m_renderDistance; z <= playerChunkPosZ + m_renderDistance; z++)
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
        unsigned int size = m_chunksToRender.size();
        for (unsigned int i = 0; i < size; i++)
        {
            m_chunksToRender.get(i)->draw(shader);
        }
    }

    void World::updateChunksThread()
    {
        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
        while (true)
        {
            while (!m_chunksToBuild.empty())
            {
                // std::cout << "Popping" << std::endl;
                Chunk *chunk = m_chunksToBuild.pop();
                chunk->constructVBO();
                m_chunksToRender.push(chunk);
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
