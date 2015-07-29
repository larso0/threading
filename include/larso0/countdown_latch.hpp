//
// Created by larso on 17.07.15.
//

#ifndef THREADING_COUNTDOWN_LATCH_HPP
#define THREADING_COUNTDOWN_LATCH_HPP

#include <mutex>

#ifdef __MINGW32__
	#include "mingw.mutex.h"
	#include "mingw.condition_variable.h"
#else
	#include <condition_variable>
#endif

namespace larso0
{

    class countdown_latch
    {
    public:
        countdown_latch(int n = 0) : n(n) {}

        inline void wait()
        {
            std::unique_lock<std::mutex> lk(mut);
            if(n > 0)
            {
                cond.wait(lk);
            }
        }

        inline void countdown()
        {
            std::unique_lock<std::mutex> lk(mut);
            n--;
            if(n == 0)
            {
                cond.notify_all();
            }
        }

    private:
        int n;
        std::mutex mut;
        std::condition_variable cond;
    };

} /* namespace larso0 */

#endif //THREADING_COUNTDOWN_LATCH_HPP
