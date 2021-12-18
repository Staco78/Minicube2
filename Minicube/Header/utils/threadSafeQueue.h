#pragma once

#include <mutex>
#include <queue>

namespace Minicube
{
    template <typename T>
    class ThreadSafeQueue
    {
    public:
        bool empty()
        {
            m_mutex.lock();
            bool ret = m_list.empty();
            m_mutex.unlock();
            return ret;
        }
        void push(const T &data)
        {
            m_mutex.lock();
            m_list.push_back(data);
            m_mutex.unlock();
        }

        int erase(const T &data)
        {
            m_mutex.lock();
            auto it = m_list.begin();
            int r = 0;
            while (it != m_list.end())
            {
                if (*it == data)
                {
                    m_list.erase(it);
                    r = 1;
                    break;
                }
                it++;
            }
            m_mutex.unlock();
            return r;
        }

        T pop()
        {
            m_mutex.lock();
            T data = m_list.back();
            m_list.pop_back();
            m_mutex.unlock();
            return data;
        }

    private:
        std::vector<T> m_list;
        std::mutex m_mutex;
    };
} // namespace Minicube
