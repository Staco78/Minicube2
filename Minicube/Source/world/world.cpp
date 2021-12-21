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
                ChunkState state = it->second->getState();
                if (state != CHUNK_STATE_LOADING)
                {
                    delete it->second;
                    it = m_chunks.erase(it);
                }
                else
                    it++;
            }
            else
                it++;
        }
        m_chunks.unlock();

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

    void World::draw(const Shader &shader)
    {
        m_chunks.lock();
        for (auto &chunk : m_chunks)
        {
            chunk.second->draw(shader);
        }
        m_chunks.unlock();
    }

    void World::updateChunksThread()
    {
        while (true)
        {

            static std::vector<Chunk *> chunks;
            static glm::vec3 lastPlayerChunkPos;
            static int lastChunkMapSize = 0;

            for (unsigned int i = 0; i < m_chunks.size(); i++)
            {

                if (i % 10 == 0)
                {
                    glm::ivec3 camPos = m_camera->getPosition();
                    glm::vec3 playerChunkPos = glm::vec3(camPos.x / 16, camPos.y / 16, camPos.z / 16);
                    int chunkMapSize = m_chunks.size();

                    if (lastPlayerChunkPos != playerChunkPos || lastChunkMapSize != chunkMapSize)
                    {
                        lastPlayerChunkPos = playerChunkPos;
                        lastChunkMapSize = chunkMapSize;

                        chunks.clear();

                        m_chunks.lock();
                        for (auto it = m_chunks.begin(); it != m_chunks.end(); it++)
                        {
                            chunks.push_back(it->second);
                        }
                        m_chunks.unlock();

                        std::sort(chunks.begin(), chunks.end(), [playerChunkPos](Chunk *a, Chunk *b)
                                  {
                                glm::vec3 posA = a->getPos();
                                glm::vec3 posB = b->getPos();
                                return (glm::dot(playerChunkPos - posB, playerChunkPos - posB) > glm::dot(playerChunkPos - posA, playerChunkPos - posA)); });

                        i = 0;
                    }
                }

                Chunk *chunk = chunks[i];
                if (chunk->getFlags() & CHUNK_FLAG_NEED_REBUILD)
                {
                    chunk->build();
                }
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void World::startThreads()
    {
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
