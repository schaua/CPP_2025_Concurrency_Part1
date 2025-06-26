#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;


double current_balance{1000};
std::string message;
std::mutex bal_mut{};

void deposit(double amount){
    std::this_thread::sleep_for(100ms);
    {    
        std::unique_lock<std::mutex> lg{bal_mut};
        current_balance += amount;
    }
    std::this_thread::sleep_for(100ms);
}

void withdraw(double amount){
    std::this_thread::sleep_for(100ms);
    {    
        std::unique_lock<std::mutex> lg{bal_mut};
        current_balance -= amount;
        std::this_thread::sleep_for(1000ms);
    }
}

int main(int argc, char const *argv[])
{
    {
        std::unique_lock<std::mutex> lg{bal_mut};
        std::cout << "Current balance: " << current_balance << std::endl;
    }
    for(int t = 3; t ; --t){
        // std::thread deposit_thread = std::thread{deposit, 100};
        // std::thread withdraw_thread = std::thread{withdraw, 200};
        std::thread deposit_thread{[&](double amount){deposit(amount); message="Hello";},100};
        std::thread withdraw_thread{[&](double amount){withdraw(amount);},200};

        // scope block
        {
            std::unique_lock<std::mutex> lg{bal_mut, std::defer_lock};
            if (lg.try_lock())
            {
                std::cout << "Current balance: before " << current_balance << std::endl;
                std::cout << "Unlock the resoure for update" << std::endl;
                lg.unlock();
                std::this_thread::sleep_for(100ms);
                lg.lock();
                std::cout << "Current balance: after " << current_balance << std::endl;

            }
            else
                std::cout << "Unable to obtain current_balance lock" << std::endl;

        }

        deposit_thread.join();
        withdraw_thread.join();
    }
    {
        std::unique_lock<std::mutex> lg{bal_mut};
        std::cout << "Final balance: " << current_balance << std::endl;
    }
    return 0;
}