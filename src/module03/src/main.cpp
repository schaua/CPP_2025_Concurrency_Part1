#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <vector>
#include <shared_mutex>
#include <condition_variable>


using namespace std::chrono_literals;

std::condition_variable cv;

std::vector<std::string> transactions;
std::shared_mutex transaction_mut;

double current_balance{1000};
std::string message;
std::mutex bal_mut{};

void deposit(double amount){
    std::this_thread::sleep_for(100ms);
    {    
        std::scoped_lock<std::shared_mutex, std::mutex> lg{ transaction_mut, bal_mut};
        current_balance += amount;
        transactions.push_back("Added money to the account: ");
        cv.notify_all();
    }
    std::this_thread::sleep_for(100ms);
}

void withdraw(double amount){
    std::this_thread::sleep_for(100ms);
    {    
        std::scoped_lock lg{bal_mut, transaction_mut};
        current_balance -= amount;
        transactions.push_back("Removed money from the account: ");
        cv.notify_all();
        std::this_thread::sleep_for(1000ms);
    }
}

void statement() {
    std::unique_lock<std::mutex> ul{bal_mut};
    std::cout << "About to wait" << std::endl;
    cv.wait(ul, [] { return !transactions.empty();});    
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
        cv.notify_all();
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