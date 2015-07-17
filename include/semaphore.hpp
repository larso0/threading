//
// Created by larso on 17.07.15.
//

#ifndef THREADING_SEMAPHORE_HPP
#define THREADING_SEMAPHORE_HPP

#include <mutex>
#include <condition_variable>

namespace larso0
{

    class semaphore
    {
    public:
        semaphore(int n = 0) : n(n) {}

        inline void notify()
        {
            std::unique_lock<std::mutex> lk(mut);
            n++;
            cond.notify_one();
        }

        inline void wait()
        {
            std::unique_lock<std::mutex> lk(mut);
            while(n == 0)
            {
                cond.wait(lk);
            }
            n--;
        }
    private:
        int n;
        std::mutex mut;
        std::condition_variable cond;
    };

} /* namespace larso0 */

#endif //THREADING_SEMAPHORE_HPP
