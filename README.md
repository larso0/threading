# threading
Some useful threading stuff for C++11.
### Example of how to use the threading pool and countdown_latch.
```
#include <iostream>
#include <mutex>
#include <larso0/pool.hpp>
#include <larso0/countdown_latch.hpp>

using std::cout;
using std::mutex;
using std::unique_lock;
using larso0::pool;
using larso0::countdown_latch.hpp;

const unsigned TASK_COUNT = 100;

int main()
{
    pool mypool;
    mutex mut;
    countdown_latch latch(TASK_COUNT);
    
    //Schedule the tasks that the thread pool should perform.
    for(unsigned i = 0; i < TASK_COUNT; i++)
    {
        //The pool takes a c++11 closure as parameter (function<void()>).
        mypool.schedule([&mut, &latch, i]() //Capture the mutex and latch by reference.
                                            //Capture the index by value.
            {
                { //Lock the mutex before using cout (not thread safe).
                    unique_lock<mutex> lk(mut);
                    cout << "Task number " << i << '\n';
                }
                //Countdown the latch when done performing the task.
                latch.countdown();
            }
        );
    }
    
    //Wait until all tasks are finished.
    latch.wait();
    return 0;
}
```
