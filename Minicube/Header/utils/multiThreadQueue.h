#pragma once

#include <mutex>
#include <queue>

namespace Minicube
{
    template <typename T>
    class MultiThreadQueue
    {
    public:
        bool empty()
        {
            m_mutex.lock();
            bool ret = m_queue.empty();
            m_mutex.unlock();
            return ret;
        }
        void push(const T &data)
        {
            m_mutex.lock();
            m_queue.push(data);
            m_mutex.unlock();
        }

        T pop()
        {
            m_mutex.lock();
            T data = m_queue.front();
            m_queue.pop();
            m_mutex.unlock();
            return data;
        }

    private:
        std::queue<T> m_queue;
        std::mutex m_mutex;
    };
} // namespace Minicube
