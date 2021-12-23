#include "world/world.h"

namespace Minicube
{
    World::World(Camera *camera)
    {
        m_camera = camera;
    }

    void World::updateVisibleChunks()
    {
        glm::vec3 camPos = m_camera->getPosition();
        int playerChunkPosX = int(camPos.x) / 16;
        int playerChunkPosZ = int(camPos.z) / 16;

        for (int x = playerChunkPosX - m_renderDistance; x <= playerChunkPosX + m_renderDistance; x++)
        {
            for (int z = playerChunkPosZ - m_renderDistance; z <= playerChunkPosZ + m_renderDistance; z++)
            {
                if (getChunk(glm::ivec3(x, 0, z)) == nullptr)
                {
                    HeightMap heightMap(x, z, &m_perlinNoiseContext);
                    int nbChunks = heightMap.maxHeight / 16 + 1;
                    for (int y = 0; y < nbChunks; y++)
                    {
                        glm::ivec3 pos(x, y, z);
                        Chunk *chunk = new Chunk(&m_chunks, pos);
                        chunk->generate(&heightMap);
                        m_chunks.set(pos, chunk);
                    }
                }
            }
        }
    }

    inline bool isVisible(const glm::ivec3 &chunkPos, int playerChunkPosX, int playerChunkPosZ, int renderDistance)
    {
        return chunkPos.x >= playerChunkPosX - renderDistance && chunkPos.x <= playerChunkPosX + renderDistance &&
               chunkPos.z >= playerChunkPosZ - renderDistance && chunkPos.z <= playerChunkPosZ + renderDistance;
    }

    void World::draw(const Shader &shader)
    {
        glm::vec3 camPos = m_camera->getPosition();
        int playerChunkPosX = int(camPos.x) / 16;
        int playerChunkPosZ = int(camPos.z) / 16;

        m_chunks.lock();
        auto it = m_chunks.begin();
        while (it != m_chunks.end())
        {
            if (!isVisible(it->first, playerChunkPosX, playerChunkPosZ, m_renderDistance))
            {
                if (it->second->getFlags() & CHUNK_FLAG_IS_BUILDING)
                    it++;
                else
                {
                    delete it->second;
                    it = m_chunks.erase(it);
                }
            }
            else
            {
                if (it->second->getFlags() & CHUNK_FLAG_NEED_INIT)
                    it->second->init();
                it->second->draw(shader);
                it++;
            }
        }
        m_chunks.unlock();
    }

    void World::updateChunksThread()
    {
        while (true)
        {

            static std::vector<Chunk *> chunks;
            static glm::vec2 lastPlayerChunkPos;
            static int lastChunkMapSize = 0;

            if (chunks.size() == 0)
            {
                m_chunks.lock();
                for (auto it = m_chunks.begin(); it != m_chunks.end(); it++)
                {
                    chunks.push_back(it->second);
                }
                m_chunks.unlock();
            }

            for (unsigned int i = 0; i < chunks.size(); i++)
            {

                if ((i % 100 == 0 && i != 0) || lastChunkMapSize == 0)
                {
                    glm::ivec3 camPos = m_camera->getPosition();
                    glm::vec2 playerChunkPos = glm::vec2(camPos.x / 16, camPos.z / 16);
                    int chunkMapSize = m_chunks.size();

                    if (lastPlayerChunkPos != playerChunkPos || lastChunkMapSize != chunkMapSize)
                    {
                        lastPlayerChunkPos = playerChunkPos;
                        lastChunkMapSize = chunkMapSize;

                        chunks.clear();
                        chunks.shrink_to_fit();

                        m_chunks.lock();
                        for (auto it = m_chunks.begin(); it != m_chunks.end(); it++)
                        {
                            chunks.push_back(it->second);
                        }
                        std::sort(chunks.begin(), chunks.end(), [playerChunkPos](Chunk *a, Chunk *b)
                                  {
                                glm::vec2 posA(a->getPos().x, a->getPos().z);
                                glm::vec2 posB(b->getPos().x, b->getPos().z);
                                return (glm::dot(playerChunkPos - posB, playerChunkPos - posB) > glm::dot(playerChunkPos - posA, playerChunkPos - posA)); });
                        m_chunks.unlock();

                        i = 0;
                    }
                }

                if (chunks[i]->getFlags() & CHUNK_FLAG_NEED_REBUILD)
                    chunks[i]->build();
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void World::generateChunksThread()
    {
        while (true)
        {
            updateVisibleChunks();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void World::startThreads()
    {
        std::thread generateChunksThread(&World::generateChunksThread, this);
        generateChunksThread.detach();
        std::thread updateChunksThread(&World::updateChunksThread, this);
        updateChunksThread.detach();
    }

    inline Chunk *World::getChunk(const glm::ivec3 &pos)
    {
        return m_chunks.get(pos);
    }

    Block *World::getBlock(int x, int y, int z)
    {

        Chunk *chunk = getChunk(glm::ivec3(x / 16, y / 16, z / 16));

        int _x;
        int _y;
        int _z;

        if (x % 16 == 0)
            _x = 0;
        else if (x >= 0)
            _x = x % 16;
        else
            _x = 16 + (x % 16);

        if (y % 16 == 0)
            _y = 0;
        else if (y >= 0)
            _y = y % 16;
        else
            _y = 16 + (y % 16);

        if (z % 16 == 0)
            _z = 0;
        else if (z >= 0)
            _z = z % 16;
        else
            _z = 16 + (z % 16);

        return chunk->getBlock(_x, _y, _z);
    }

} // namespace Minicube
