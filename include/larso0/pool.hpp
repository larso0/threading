//
// Created by larso on 17.07.15.
//

#ifndef THREADING_POOL_HPP
#define THREADING_POOL_HPP

#include <thread>
#include <mutex>
#include <deque>
#include <vector>
#include <atomic>
#include <functional>

#include "semaphore.hpp"

namespace larso0
{

    class pool
    {
    public:
        pool(unsigned cpunum = std::thread::hardware_concurrency())
        {
            stop = false;
            for(unsigned i = 0; i < cpunum; i++)
            {
                workers.push_back(std::thread([&]{
                    std::function<void()> task;
                    while(true)
                    {
                        sem.wait();
                        if(stop) break;
                        {
                            std::unique_lock<std::mutex> lk(mut);
                            task = tasks.front();
                            tasks.pop_front();
                        }
                        task();
                    }
                }));
            }
        }

        ~pool()
        {
            stop = true;
            for(unsigned i = 0; i < workers.size(); i++)
            {
                sem.notify();
            }
            for(unsigned i = 0; i < workers.size(); i++)
            {
                workers[i].join();
            }
        }

        inline void schedule(std::function<void()> f)
        {
            if(stop) return;
            {
                std::unique_lock<std::mutex> lk(mut);
                tasks.push_back(f);
            }
            sem.notify();
        }

        inline unsigned worker_count()
        {
            return workers.size();
        }
    private:
        std::vector<std::thread> workers;
        std::deque<std::function<void()>> tasks;
        std::mutex mut;
        larso0::semaphore sem;
        std::atomic<bool> stop;
    };

} /* namespace larso0 */

#endif //THREADING_POOL_HPP
