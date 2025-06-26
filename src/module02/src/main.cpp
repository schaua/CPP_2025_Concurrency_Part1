#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <future>
#include <stdexcept>

using namespace std::chrono_literals;

void computeValue(std::promise<int> prom, int x){
    try {
        if (x < 0)
            throw std::invalid_argument{"No negative values allowed"};
        prom.set_value(x * 42);
    } catch(...)
    {
        prom.set_exception(std::current_exception());
    }

}

int main(int argc, char const *argv[])
{
    std::promise<int> prom1;
    std::future<int> fut = prom1.get_future();

    std::thread t{computeValue, std::move(prom1), -5};

    
    try {
        std::cout << fut.get() << std::endl;
    }
    catch (std::exception& exp)
    {
        std::cout << "An exception occurred " << exp.what() <<  std::endl;
    }
    t.join();

    return 0;
}




// double current_balance{1000};
// std::string message;
// std::mutex bal_mut{};

// void deposit(double amount){
//     std::this_thread::sleep_for(1000ms);
//     {    
//         std::lock_guard<std::mutex> lg{bal_mut};
//         current_balance += amount;
//     }
//     std::this_thread::sleep_for(1000ms);
// }

// void withdraw(double amount){
//     std::this_thread::sleep_for(1000ms);
//     {    
//         std::lock_guard<std::mutex> lg{bal_mut};
//         current_balance -= amount;
//     }
//     std::this_thread::sleep_for(1000ms);
// }

// int main(int argc, char const *argv[])
// {
//     {
//         std::lock_guard<std::mutex> lg{bal_mut};
//         std::cout << "Current balance: " << current_balance << std::endl;
//     }
//     for(int t = 3; t ; --t){
//         // std::thread deposit_thread = std::thread{deposit, 100};
//         // std::thread withdraw_thread = std::thread{withdraw, 200};
//         std::thread deposit_thread{[&](double amount){deposit(amount); message="Hello";},100};
//         std::thread withdraw_thread{[&](double amount){withdraw(amount);},200};
//         deposit_thread.join();
//         withdraw_thread.join();
//     }
//     {
//         std::lock_guard<std::mutex> lg{bal_mut};
//         std::cout << "Current balance: " << current_balance << std::endl;
//     }
//     return 0;
// }
