#include <atomic>
#include <vector>
#include <thread>
#include <iostream>

class SpinLock {
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
public:
    void lock() {
        while (flag.test_and_set(std::memory_order_acquire)){}
    }
    void unlock() {
        flag.clear(std::memory_order_release);
    }
};

int global_count{0};
SpinLock sp;
int main(int argc, char const *argv[])
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i){
        threads.push_back(std::thread{[]{
            sp.lock();
            global_count++;
            sp.unlock();
        }});
    }
    for (auto& thread : threads)
        thread.join();
    std::cout << "Global Count: " << global_count << std::endl;

    return 0;
}
