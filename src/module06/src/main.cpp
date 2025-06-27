#include <iostream>
#include <thread>
#include <semaphore>
#include <chrono>

std::counting_semaphore<3> semaphore{3};

void accessResource(int id){
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
        
    for (auto& thread : threads)
        thread.join();
    return 0;
}



