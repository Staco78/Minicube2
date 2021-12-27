#pragma once

#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>

#include "Glew/glew.h"

#include <Glm/glm.hpp>
#include "Glm/vec3.hpp"

#include "chunkMap.h"
#include "shader.h"
#include "utils.h"
#include "renderer/textures.h"
#include "gen/worldGen.h"
#include "block.h"

namespace Minicube
{

    class DynamicVBO
    {
    public:
        void init()
        {
            glGenBuffers(1, &ID);
        }
        void sendData()
        {
            m_mutex.lock();
            size = m_data->size();
            glBindBuffer(GL_ARRAY_BUFFER, ID);
            glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), m_data->data(), GL_STATIC_DRAW);
            delete m_data;
            m_data = new std::vector<float>();
            m_mutex.unlock();
        }

        inline void push_back(float data)
        {
            m_mutex.lock();
            m_data->push_back(data);
            m_mutex.unlock();
        }

        inline int getSize()
        {
            return size;
        }

        ~DynamicVBO()
        {
            glDeleteBuffers(1, &ID);
            delete m_data;
        }

        inline unsigned int getTrianglesCount() { return (getSize() / 5); }
        inline unsigned int getID() { return ID; }

    private:
        size_t size = 0;
        std::mutex m_mutex;
        unsigned int ID = 0;
        std::vector<float> *m_data = new std::vector<float>();
    };

    enum ChunkFlags
    {
        CHUNK_FLAG_NEED_INIT = 1,
        CHUNK_FLAG_IS_GENERATED = 2,
        CHUNK_FLAG_NEED_REBUILD = 4,
        CHUNK_FLAG_IS_BUILDING = 8,
        CHUNK_FLAG_NEED_SEND_VBO = 16,
        CHUNK_FLAG_NEED_DELETE = 32,
    };

    class Chunk
    {
    public:
        Chunk(ChunkMap *chunkMap, glm::ivec3 pos);
        void init();
        ~Chunk()
        {
            m_flags |= CHUNK_FLAG_NEED_DELETE;

            m_isConstructing_mutex.lock();
            m_isConstructing_mutex.unlock();
            free(m_blocks);
            glDeleteVertexArrays(1, &m_VAO);
            ;
        }
        void draw(const Shader &shader);
        void addBlock(int x, int y, int z, Block &block);
        Block *getBlock(int x, int y, int z);
        inline const glm::ivec3 &getPos() const
        {
            return m_pos;
        }

        void generate(WorldGen::HeightMap *heightMap);

        inline int getFlags() const
        {
            return m_flags;
        }

        inline void setFlags(int flags)
        {
            m_flags = flags;
        }

        inline void addFlag(int flag)
        {
            m_flags |= flag;
        }

        inline WorldGen::HeightMap *getHeightMap()
        {
            return m_heightMap;
        }

        void build();

    private:
        void constructVBO();

        inline int getBlockIndex(int x, int y, int z)
        {
            assert(x >= 0 && x < 16 && y >= 0 && y < 16 && z >= 0 && z < 16);
            return x + y * 16 + z * 256;
        }

        inline glm::uvec3 getBlockPos(int index)
        {
            assert(index >= 0 && index < 16 * 16 * 16);
            return glm::uvec3(index % 16, index / 16 % 16, index / 256);
        }

        Block *getBlockInWorld(int x, int y, int z);

        Block *m_blocks = nullptr;
        glm::ivec3 m_pos;
        DynamicVBO m_VBO;
        unsigned int m_VAO = 0;
        glm::mat4 m_model = glm::mat4(1.0f);
        ChunkMap *m_chunkMap = nullptr;
        WorldGen::HeightMap *m_heightMap = nullptr;

        std::atomic<int> m_flags = CHUNK_FLAG_NEED_INIT;
        std::mutex m_isConstructing_mutex;
    };

} // namespace Minicube
