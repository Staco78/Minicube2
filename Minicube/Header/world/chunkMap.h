#pragma once

#include <map>
#include <iostream>
#include <mutex>

#include "Glm/vec3.hpp"

namespace std
{
    template <>
    struct less<glm::ivec3>
    {
        bool operator()(const glm::ivec3 &left, const glm::ivec3 &right) const
        {
            if (left.y == right.y && left.x == right.x)
                return left.z < right.z;
            else if (left.x == right.x)
                return left.y < right.y;
            else
                return left.x < right.x;
        }
    };
}

namespace Minicube
{

    class Chunk;

    class ChunkMap : public std::map<glm::ivec3, Chunk *>
    {
    public:
        Chunk *get(glm::ivec3 key)
        {
            m_mutex.lock();
            auto it = find(key);
            if (it == end())
            {
                m_mutex.unlock();
                return nullptr;
            }
            m_mutex.unlock();
            return it->second;
        }

        void set(const glm::ivec3 &key, Chunk *value)
        {
            m_mutex.lock();
            operator[](key) = value;
            m_mutex.unlock();
        }

        void lock()
        {
            m_mutex.lock();
        }

        void unlock()
        {
            m_mutex.unlock();
        }

        ~ChunkMap()
        {
            for (auto it = this->begin(); it != this->end(); it++)
                delete it->second;
        }

    private:
        std::mutex m_mutex;
    };
}