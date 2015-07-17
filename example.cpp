#include <iostream>
#include <mutex>

#include "larso0/pool.hpp"
#include "larso0/countdown_latch.hpp"

using namespace std;

const int N = 100;

int main()
{
    larso0::pool pool;
    larso0::countdown_latch latch(N);
    std::mutex out_mutex;

    for(int i = 0; i < N; i++)
    {
        pool.schedule([&out_mutex, &latch, i]{
            {
                unique_lock<mutex> lk(out_mutex);
                cout << "Task " << i << endl;
            }
            latch.countdown();
        });
    }

    latch.wait();
    return 0;
}