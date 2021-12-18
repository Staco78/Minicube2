#pragma once

#include <vector>
#include <mutex>

namespace Minicube
{
    template <typename T>
    class ThreadSafeList
    {
    public:
        unsigned int size()
        {
            m_mutex.lock();
            unsigned int ret = m_data.size();
            m_mutex.unlock();
            return ret;
        }

        T get(unsigned int index)
        {
            m_mutex.lock();
            T data = m_data[index];
            m_mutex.unlock();
            return data;
        }

        void set(unsigned int index, T data)
        {
            m_mutex.lock();
            m_data[index] = data;
            m_mutex.unlock();
        }

        void push(T data)
        {
            m_mutex.lock();
            m_data.push_back(data);
            m_mutex.unlock();
        }

        int erase(T data)
        {
            m_mutex.lock();
            auto it = m_data.begin();
            int r = 0;
            while (it != m_data.end())
            {
                if (*it == data)
                {
                    m_data.erase(it);
                    r = 1;
                    break;
                }
                it++;
            }
            m_mutex.unlock();
            return r;
        }

    private:
        std::vector<T> m_data;
        std::mutex m_mutex;
    };
} // namespace Minicube
