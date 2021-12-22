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
#include "gen/heightMap.h"

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
            glBindBuffer(GL_ARRAY_BUFFER, ID);
            glBufferData(GL_ARRAY_BUFFER, m_data.size() * sizeof(float), m_data.data(), GL_DYNAMIC_DRAW);
            m_mutex.unlock();
        }
        void push_back(float data)
        {
            m_mutex.lock();
            m_data.push_back(data);
            m_mutex.unlock();
        }
        int getSize()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_data.size();
        }

        void clear()
        {
            m_mutex.lock();
            m_data.clear();
            m_mutex.unlock();
        }

        unsigned int getTrianglesCount() { return (getSize() / 5); }
        unsigned int getID() { return ID; }
        std::vector<float> *getData() { return &m_data; }

    private:
        std::mutex m_mutex;
        unsigned int ID;
        std::vector<float> m_data;
    };

    typedef struct
    {
        uint16_t id;
    } Block;

    enum class BLockType
    {
        AIR,
        GRASS,
        STONE
    };

    enum ChunkState
    {
        CHUNK_STATE_DESTROYING = -1,
        CHUNK_STATE_0,
        CHUNK_STATE_UNLOADED,
        CHUNK_STATE_LOADING,
        CHUNK_STATE_LOADED,
        CHUNK_STATE_IDLE,
    };

    enum ChunkFlags
    {
        CHUNK_FLAG_NONE = 0,
        CHUNK_FLAG_NEED_INIT = 1,
        CHUNK_FLAG_NEED_GENERATION = 2,
        CHUNK_FLAG_NEED_REBUILD = 4
    };

    class Chunk
    {
    public:
        Chunk(ChunkMap *chunkMap, glm::ivec3 pos);
        void init();
        ~Chunk()
        {
            while (m_state == CHUNK_STATE_LOADING)
            {
            }
            m_state = CHUNK_STATE_DESTROYING;

            free(m_blocks);
            // std::cout << "chunk destructed\n";
        }
        void draw(const Shader &shader);
        void addBlock(int x, int y, int z, Block &block);
        Block *getBlock(int x, int y, int z);
        inline glm::ivec3 getPos()
        {
            return m_pos;
        }

        void generate(HeightMap *heightMap);

        inline ChunkState getState()
        {
            return m_state;
        }

        inline int getFlags()
        {
            return m_flags;
        }

        inline void setFlags(int flags)
        {
            m_flags = flags;
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

        Block *m_blocks;
        glm::ivec3 m_pos;
        DynamicVBO m_VBO;
        unsigned int m_VAO;
        glm::mat4 m_model = glm::mat4(1.0f);
        ChunkMap *m_chunkMap = nullptr;

        std::atomic<ChunkState> m_state = CHUNK_STATE_0;
        std::atomic<int> m_flags = CHUNK_FLAG_NEED_INIT;
    };

} // namespace Minicube

namespace
{

    enum Side
    {
        BACK = 0b00000001,
        FRONT = 0b00000010,
        TOP = 0b00000100,
        BOTTOM = 0b00001000,
        LEFT = 0b00010000,
        RIGHT = 0b00100000
    };

    void get_block_faces(const glm::uvec3 &pos, Minicube::DynamicVBO &VBO, Side side, unsigned int textureId)
    {

        if (side & BACK)
        {
            for (unsigned int i = 0; i < Minicube::Vertices::cube::back.size(); i += 5)
            {
                VBO.push_back(Minicube::Vertices::cube::back.at(i) + pos.x);
                VBO.push_back(Minicube::Vertices::cube::back.at(i + 1) + pos.y);
                VBO.push_back(Minicube::Vertices::cube::back.at(i + 2) + pos.z);
                VBO.push_back(Minicube::Vertices::cube::back.at(i + 3));
                VBO.push_back(Minicube::Vertices::cube::back.at(i + 4));
                VBO.push_back(textureId);
                VBO.push_back(0.0f);
            }
        }
        if (side & FRONT)
        {
            for (unsigned int i = 0; i < Minicube::Vertices::cube::front.size(); i += 5)
            {
                VBO.push_back(Minicube::Vertices::cube::front.at(i) + pos.x);
                VBO.push_back(Minicube::Vertices::cube::front.at(i + 1) + pos.y);
                VBO.push_back(Minicube::Vertices::cube::front.at(i + 2) + pos.z);
                VBO.push_back(Minicube::Vertices::cube::front.at(i + 3));
                VBO.push_back(Minicube::Vertices::cube::front.at(i + 4));
                VBO.push_back(textureId);
                VBO.push_back(1.0f);
            }
        }
        if (side & TOP)
        {
            for (unsigned int i = 0; i < Minicube::Vertices::cube::top.size(); i += 5)
            {
                VBO.push_back(Minicube::Vertices::cube::top.at(i) + pos.x);
                VBO.push_back(Minicube::Vertices::cube::top.at(i + 1) + pos.y);
                VBO.push_back(Minicube::Vertices::cube::top.at(i + 2) + pos.z);
                VBO.push_back(Minicube::Vertices::cube::top.at(i + 3));
                VBO.push_back(Minicube::Vertices::cube::top.at(i + 4));
                VBO.push_back(textureId);
                VBO.push_back(4.0f);
            }
        }
        if (side & BOTTOM)
        {
            for (unsigned int i = 0; i < Minicube::Vertices::cube::bottom.size(); i += 5)
            {
                VBO.push_back(Minicube::Vertices::cube::bottom.at(i) + pos.x);
                VBO.push_back(Minicube::Vertices::cube::bottom.at(i + 1) + pos.y);
                VBO.push_back(Minicube::Vertices::cube::bottom.at(i + 2) + pos.z);
                VBO.push_back(Minicube::Vertices::cube::bottom.at(i + 3));
                VBO.push_back(Minicube::Vertices::cube::bottom.at(i + 4));
                VBO.push_back(textureId);
                VBO.push_back(5.0f);
            }
        }
        if (side & LEFT)
        {
            for (unsigned int i = 0; i < Minicube::Vertices::cube::left.size(); i += 5)
            {
                VBO.push_back(Minicube::Vertices::cube::left.at(i) + pos.x);
                VBO.push_back(Minicube::Vertices::cube::left.at(i + 1) + pos.y);
                VBO.push_back(Minicube::Vertices::cube::left.at(i + 2) + pos.z);
                VBO.push_back(Minicube::Vertices::cube::left.at(i + 3));
                VBO.push_back(Minicube::Vertices::cube::left.at(i + 4));
                VBO.push_back(textureId);
                VBO.push_back(2.0f);
            }
        }
        if (side & RIGHT)
        {
            for (unsigned int i = 0; i < Minicube::Vertices::cube::right.size(); i += 5)
            {
                VBO.push_back(Minicube::Vertices::cube::right.at(i) + pos.x);
                VBO.push_back(Minicube::Vertices::cube::right.at(i + 1) + pos.y);
                VBO.push_back(Minicube::Vertices::cube::right.at(i + 2) + pos.z);
                VBO.push_back(Minicube::Vertices::cube::right.at(i + 3));
                VBO.push_back(Minicube::Vertices::cube::right.at(i + 4));
                VBO.push_back(textureId);
                VBO.push_back(3.0f);
            }
        }
    }
}