#include "world/world.h"

namespace Minicube
{
    World::World(Camera *camera) : m_perlinNoiseContext(time(0))
    {
        m_camera = camera;
        WorldGen::init();
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
                if (getVerticalChunk(glm::ivec2(x, z)) == nullptr)
                {
                    VerticalChunk *chunk = new VerticalChunk(glm::ivec2(x, z), &m_chunks);
                    chunk->generate(&m_perlinNoiseContext);
                    m_chunks.set(glm::ivec2(x, z), chunk);
                }
                camPos = m_camera->getPosition();
                playerChunkPosX = int(camPos.x) / 16;
                playerChunkPosZ = int(camPos.z) / 16;
            }
        }
    }

    inline bool isVisible(const glm::ivec2 &chunkPos, int playerChunkPosX, int playerChunkPosZ, int renderDistance)
    {
        return chunkPos.x >= playerChunkPosX - renderDistance && chunkPos.x <= playerChunkPosX + renderDistance &&
               chunkPos.y >= playerChunkPosZ - renderDistance && chunkPos.y <= playerChunkPosZ + renderDistance;
    }

    void World::draw(const Shader &shader)
    {
        glm::vec3 camPos = m_camera->getPosition();
        int playerChunkPosX = int(camPos.x) / 16;
        int playerChunkPosZ = int(camPos.z) / 16;

        m_chunks.lock();
        auto it1 = m_chunks.begin();
        while (it1 != m_chunks.end())
        {
            auto it2 = it1->second->begin();
            bool addIt = true;
            while (it2 != it1->second->end())
            {
                if (!isVisible(it1->first, playerChunkPosX, playerChunkPosZ, m_renderDistance))
                {
                    if ((*it2)->getFlags() & CHUNK_FLAG_IS_BUILDING)
                        it2++;
                    else
                    {
                        it2 = it1->second->erase(it2);
                        if (it1->second->size() == 0)
                        {
                            delete it1->second;
                            it1 = m_chunks.erase(it1);
                            addIt = false;
                            break;
                        }
                    }
                }
                else
                {
                    if ((*it2)->getFlags() & CHUNK_FLAG_NEED_INIT)
                        (*it2)->init();
                    (*it2)->draw(shader);
                    it2++;
                }
            }
            if (addIt)
                it1++;
        }
        m_chunks.unlock();
    }

    void World::updateChunksThread()
    {
        while (true)
        {

            static std::vector<VerticalChunk *> chunks;
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

                if (i % 10 == 0 || lastChunkMapSize == 0)
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
                        std::sort(chunks.begin(), chunks.end(), [playerChunkPos](VerticalChunk *a, VerticalChunk *b)
                                  {
                                glm::vec2 posA(a->getPos().x, a->getPos().y);
                                glm::vec2 posB(b->getPos().x, b->getPos().y);
                                return (glm::dot(playerChunkPos - posB, playerChunkPos - posB) > glm::dot(playerChunkPos - posA, playerChunkPos - posA)); });
                        m_chunks.unlock();

                        i = 0;
                        if (chunks.size() == 0)
                            continue;
                    }
                }

                for (auto it = chunks[i]->begin(); it != chunks[i]->end(); it++)
                {
                    if ((*it)->getFlags() & CHUNK_FLAG_NEED_REBUILD)
                        (*it)->build();
                }
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

    Block *World::getBlock(int x, int y, int z)
    {

        Chunk *chunk = getChunk(glm::ivec3(x / 16, y / 16, z / 16));

        unsigned int _x;
        unsigned int _y;
        unsigned int _z;

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
