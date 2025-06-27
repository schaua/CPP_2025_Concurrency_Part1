#include <iostream>
#include <thread>
#include <chrono>
#include <exception>

using namespace std::chrono_literals;

void doWork() {
    try{
    std::cout << "I am working..." << std::endl;
    throw std::runtime_error("Something went wrong");
    std::cout << "...or not" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

int main(int argc, char const *argv[])
{
    std::thread t;
    try
    {
        t = std::move(std::thread{doWork});
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    t.join();
    std::cout << "This is the end of the main thread " << std::endl;
    return 0;
}
