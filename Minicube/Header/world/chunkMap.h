#pragma once

#include <map>
#include <iostream>
#include <mutex>

#include "Glm/vec2.hpp"

namespace std
{
    template <>
    struct less<glm::ivec2>
    {
        bool operator()(const glm::ivec2 &left, const glm::ivec2 &right) const
        {
            if (left.x == right.x)
                return left.y < right.y;
            else
                return left.x < right.x;
        }
    };
}

namespace Minicube
{

    class VerticalChunk;

    class ChunkMap : public std::map<glm::ivec2, VerticalChunk *>
    {
    public:
        VerticalChunk *get(glm::ivec2 key)
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

        void set(const glm::ivec2 &key, VerticalChunk *value)
        {
            m_mutex.lock();
            operator[](key) = value;
            m_mutex.unlock();
        }

        unsigned int size()
        {
            m_mutex.lock();
            unsigned int ret = std::map<glm::ivec2, VerticalChunk *>::size();
            m_mutex.unlock();
            return ret;
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