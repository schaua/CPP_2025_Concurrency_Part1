#include <iostream>
#include <thread>
#include <chrono>
#include <exception>
#include <future>

using namespace std::chrono_literals;

void doWork(std::promise<void> prom) {
    try{
    std::cout << "I am working..." << std::endl;
    throw std::runtime_error("Something went wrong");
    std::cout << "...or not" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << "thread exception" << '\n';
        prom.set_exception(std::current_exception());
    }
}

int main(int argc, char const *argv[])
{
    std::promise<void> mainPromise;
    std::future<void> mainFuture = mainPromise.get_future();

    std::thread t;
    try
    {
        t = std::move(std::thread{doWork, std::move(mainPromise)});
        mainFuture.get();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    t.join();
    std::cout << "This is the end of the main thread " << std::endl;
    return 0;
}
