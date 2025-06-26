#include <iostream>
#include <thread>

void doWork() {
    std::cout << "I am working..." << std::endl;
}

int main(int argc, char const *argv[])
{
    // std::thread not_a_real_thread{};

    auto main_thread_id = std::this_thread::get_id();

    // std::thread worker_thread{doWork};
    std::thread worker_thread([]{
            std::cout << "I am working lambda..." << std::endl;
     });

    std::cout << "This is the end of the main thread " << main_thread_id << std::endl;
    
    // For this simple app there is obviously no call to join or detach
    // but for more complex functions it might be useful to confirm that
    // the thread is able to be joined
    if (worker_thread.joinable()){
        worker_thread.join();
    }

    return 0;
}
