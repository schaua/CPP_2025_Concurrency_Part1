#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
using namespace std::chrono_literals;

double current_balance{1000};
std::string message;
std::mutex bal_mut{};

void deposit(double amount){
    std::this_thread::sleep_for(1000ms);
    bal_mut.lock();
    current_balance += amount;
    bal_mut.unlock();
    std::this_thread::sleep_for(1000ms);
}

void withdraw(double amount){
    std::this_thread::sleep_for(1000ms);
    bal_mut.lock();
    current_balance -= amount;
    bal_mut.unlock();
    std::this_thread::sleep_for(1000ms);
}

int main(int argc, char const *argv[])
{
    bal_mut.lock();
    std::cout << "Current balance: " << current_balance << std::endl;
    bal_mut.unlock();
    for(int t = 3; t ; --t){
        // std::thread deposit_thread = std::thread{deposit, 100};
        // std::thread withdraw_thread = std::thread{withdraw, 200};
        std::thread deposit_thread{[&](double amount){deposit(amount); message="Hello";},100};
        std::thread withdraw_thread{[&](double amount){withdraw(amount);},200};
        deposit_thread.join();
        withdraw_thread.join();
    }
    bal_mut.lock();
    std::cout << "Current balance: " << current_balance << std::endl;
    bal_mut.unlock();
    return 0;
}
