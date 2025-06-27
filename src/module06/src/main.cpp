#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>
#include <latch>

std::counting_semaphore<3> semaphore{3};
std::latch latch{7};

void accessResource(int id){
    latch.arrive_and_wait();
    semaphore.acquire();
    std::cout << "Thread " << id << " is accessing the resource." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Thread " << id << " is releasing the resource." << std::endl;
    semaphore.release();
}

int main(int argc, char const *argv[])
{
    std::vector<std::thread> threads;
    for (int i = 0; i < 6; ++i)
        threads.push_back(std::thread(accessResource, i));
        
    latch.arrive_and_wait();
    for (auto& thread : threads)
        thread.join();
    return 0;
}



