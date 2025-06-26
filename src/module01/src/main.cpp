#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

void doWork() {
    std::cout << "I am working..." << std::endl;
}

int main(int argc, char const *argv[])
{
    // std::thread not_a_real_thread{};

    auto main_thread_id = std::this_thread::get_id();
    
    {
        // std::thread worker_thread{doWork};
        auto worker_thread = std::jthread ([](std::stop_token stoken){
            std::cout << "I am working lambda..." ;
            while (!stoken.stop_requested()){
                // repetitive work of the thread 
                std::cout << ".";
            }    
            std::cout << std::endl;
        });

    // jthread is auto joined when it is destroyed
    // which is when it goes out scope

    std::this_thread::sleep_for(1000ms);

    worker_thread.request_stop();
    }

    std::cout << "This is the end of the main thread " << main_thread_id << std::endl;

    return 0;
}
