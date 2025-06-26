#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <vector>
#include <shared_mutex>


using namespace std::chrono_literals;


std::vector<std::string> transactions;
std::shared_mutex transaction_mut;

double current_balance{1000};
std::string message;
std::mutex bal_mut{};

void deposit(double amount){
    std::this_thread::sleep_for(100ms);
    {    
        std::unique_lock<std::mutex> lg{bal_mut};
        current_balance += amount;
        std::unique_lock<std::shared_mutex> sl{transaction_mut};
        transactions.push_back("Added money to the account: ");
    }
    std::this_thread::sleep_for(100ms);
}

void withdraw(double amount){
    std::this_thread::sleep_for(100ms);
    {    
        std::unique_lock<std::mutex> lg{bal_mut};
        current_balance -= amount;
        std::unique_lock<std::shared_mutex> sl{transaction_mut};
        transactions.push_back("Removed money from the account: ");
        std::this_thread::sleep_for(1000ms);
    }
}

void statement() {
    std::shared_lock<std::shared_mutex> sl{transaction_mut};
    for (auto& tr : transactions)
    {
        std::cout << tr << std::endl;
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

        std::thread s1{statement};
        std::thread s2{statement};
        deposit_thread.join();
        withdraw_thread.join();
        s1.join();
        s2.join();


    }
    {
        std::unique_lock<std::mutex> lg{bal_mut};
        std::cout << "Final balance: " << current_balance << std::endl;
    }
    return 0;
}