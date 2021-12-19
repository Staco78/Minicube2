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

typedef struct
{
    uint16_t id;
} Block;

namespace Minicube
{

    class DynamicVBO
    {
    public:
        DynamicVBO()
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

        unsigned int getTrianglesCount() { return (getSize() / 5); }
        unsigned int getID() { return ID; }
        std::vector<float> *getData() { return &m_data; }

    private:
        std::mutex m_mutex;
        unsigned int ID;
        std::vector<float> m_data;
    };

    class Chunk
    {
    public:
        Chunk(ChunkMap *chunkMap, glm::ivec3 pos);
        ~Chunk()
        {
            while (m_state == 2)
            {
            }
            m_state = -1;

            free(m_blocks);
            std::cout << "chunk destructed\n";
        }
        void draw(const Shader &shader);
        void addBlock(int x, int y, int z, Block &block);
        void constructVBO();
        Block *getBlock(int x, int y, int z);
        glm::ivec3 getPos()
        {
            std::lock_guard<std::mutex> lock(m_blocks_mutex);
            return m_pos;
        }

        void generate();

        int getState()
        {
            return m_state;
        }

    private:
        inline int getBlockIndex(int x, int y, int z)
        {
            return (x + y * 16 + z * 256);
        }
        inline glm::uvec3 getBlockPos(int index)
        {
            return glm::uvec3(index % 16, index / 16 % 16, index / 256);
        }

        Block *getBlockInWorld(int x, int y, int z)
        {

            if (!(x < 0 || y < 0 || z < 0 || x > 15 || y > 15 || z > 15))
                return getBlock(x, y, z);

            int _x = x + m_pos.x * 16;
            int _y = y + m_pos.y * 16;
            int _z = z + m_pos.z * 16;

            Chunk *chunk = m_chunkMap->get(glm::ivec3(floor(_x / 16.0), floor(_y / 16.0), floor(_z / 16.0)));

            if (chunk == nullptr)
                return nullptr;

            if (_x % 16 == 0)
                x = 0;
            else if (_x >= 0)
                x = _x % 16;
            else
                x = 16 + (_x % 16);

            if (_y % 16 == 0)
                y = 0;
            else if (_y >= 0)
                y = _y % 16;
            else
                y = 16 + (_y % 16);

            if (_z % 16 == 0)
                z = 0;
            else if (_z >= 0)
                z = _z % 16;
            else
                z = 16 + (_z % 16);

            return chunk->getBlock(x, y, z);
        }

        Block *m_blocks;
        glm::ivec3 m_pos;
        DynamicVBO m_VBO;
        unsigned int m_VAO;
        glm::mat4 m_model = glm::mat4(1.0f);
        ChunkMap *m_chunkMap = nullptr;

        std::mutex m_blocks_mutex;
        std::atomic_int m_state = 0;
    };

} // namespace Minicube

namespace
{

    /*
    side:
    0b00000001 = back
    0b00000010 = front
    0b00000100 = top
    0b00001000 = bottom
    0b00010000 = left
    0b00100000 = right */

    void get_block_faces(const glm::uvec3 &pos, Minicube::DynamicVBO &VBO, uint8_t side)
    {

        if (side & 0b00000001)
        {
            for (unsigned int i = 0; i < Minicube::Vertices::cube::back.size(); i += 5)
            {
                VBO.push_back(Minicube::Vertices::cube::back.at(i) + pos.x);
                VBO.push_back(Minicube::Vertices::cube::back.at(i + 1) + pos.y);
                VBO.push_back(Minicube::Vertices::cube::back.at(i + 2) + pos.z);
                VBO.push_back(Minicube::Vertices::cube::back.at(i + 3));
                VBO.push_back(Minicube::Vertices::cube::back.at(i + 4));
            }
        }
        if (side & 0b00000010)
        {
            for (unsigned int i = 0; i < Minicube::Vertices::cube::front.size(); i += 5)
            {
                VBO.push_back(Minicube::Vertices::cube::front.at(i) + pos.x);
                VBO.push_back(Minicube::Vertices::cube::front.at(i + 1) + pos.y);
                VBO.push_back(Minicube::Vertices::cube::front.at(i + 2) + pos.z);
                VBO.push_back(Minicube::Vertices::cube::front.at(i + 3));
                VBO.push_back(Minicube::Vertices::cube::front.at(i + 4));
            }
        }
        if (side & 0b00000100)
        {
            for (unsigned int i = 0; i < Minicube::Vertices::cube::top.size(); i += 5)
            {
                VBO.push_back(Minicube::Vertices::cube::top.at(i) + pos.x);
                VBO.push_back(Minicube::Vertices::cube::top.at(i + 1) + pos.y);
                VBO.push_back(Minicube::Vertices::cube::top.at(i + 2) + pos.z);
                VBO.push_back(Minicube::Vertices::cube::top.at(i + 3));
                VBO.push_back(Minicube::Vertices::cube::top.at(i + 4));
            }
        }
        if (side & 0b00001000)
        {
            for (unsigned int i = 0; i < Minicube::Vertices::cube::bottom.size(); i += 5)
            {
                VBO.push_back(Minicube::Vertices::cube::bottom.at(i) + pos.x);
                VBO.push_back(Minicube::Vertices::cube::bottom.at(i + 1) + pos.y);
                VBO.push_back(Minicube::Vertices::cube::bottom.at(i + 2) + pos.z);
                VBO.push_back(Minicube::Vertices::cube::bottom.at(i + 3));
                VBO.push_back(Minicube::Vertices::cube::bottom.at(i + 4));
            }
        }
        if (side & 0b00010000)
        {
            for (unsigned int i = 0; i < Minicube::Vertices::cube::left.size(); i += 5)
            {
                VBO.push_back(Minicube::Vertices::cube::left.at(i) + pos.x);
                VBO.push_back(Minicube::Vertices::cube::left.at(i + 1) + pos.y);
                VBO.push_back(Minicube::Vertices::cube::left.at(i + 2) + pos.z);
                VBO.push_back(Minicube::Vertices::cube::left.at(i + 3));
                VBO.push_back(Minicube::Vertices::cube::left.at(i + 4));
            }
        }
        if (side & 0b00100000)
        {
            for (unsigned int i = 0; i < Minicube::Vertices::cube::right.size(); i += 5)
            {
                VBO.push_back(Minicube::Vertices::cube::right.at(i) + pos.x);
                VBO.push_back(Minicube::Vertices::cube::right.at(i + 1) + pos.y);
                VBO.push_back(Minicube::Vertices::cube::right.at(i + 2) + pos.z);
                VBO.push_back(Minicube::Vertices::cube::right.at(i + 3));
                VBO.push_back(Minicube::Vertices::cube::right.at(i + 4));
            }
        }
    }
}