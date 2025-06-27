#include <iostream>
#include <thread>
#include <mutex>
class Account
{
private:
    int balance;
    std::recursive_mutex mutex;

public:
    Account(int initial_balance) : balance(initial_balance) {}
    void deposit(int amount)
    {
        std::lock_guard<std::recursive_mutex> lock(mutex); // RAII style lock
        balance += amount;
        // Additional complex logic...
    }
    void withdraw(int amount)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex); // RAII style lock
        balance -= amount;

        // Additional complex logic...
    }
    void transfer(Account &to, int amount)
    {
        std::unique_lock<std::recursive_mutex> lock(mutex); // RAII style lock
        // Now safely call withdraw and deposit
        this->withdraw(amount);
        to.deposit(amount);
    }
    int
    getBalance()
    {
        std::lock_guard<std::recursive_mutex> lock(mutex); // RAII style lock
        return balance;
    }
};
int main()
{
    Account account1(100);
    Account account2(50);
    std::thread t1(&Account::transfer, &account1,
                   std::ref(account2), 30);
    std::thread t2(&Account::transfer, &account2,
                   std::ref(account1), 20);
    t1.join();
    t2.join();
    std::cout << "Account 1 Balance: " << account1.getBalance()
              << std::endl;
    std::cout << "Account 2 Balance: " << account2.getBalance()
              << std::endl;

    return 0;
}

// #include <atomic>
// #include <vector>
// #include <thread>
// #include <iostream>

// class SpinLock {
//     std::atomic_flag flag = ATOMIC_FLAG_INIT;
// public:
//     void lock() {
//         while (flag.test_and_set(std::memory_order_acquire)){}
//     }
//     void unlock() {
//         flag.clear(std::memory_order_release);
//     }
// };

// int global_count{0};
// SpinLock sp;
// int main(int argc, char const *argv[])
// {
//     std::vector<std::thread> threads;
//     for (int i = 0; i < 4; ++i){
//         threads.push_back(std::thread{[]{
//             sp.lock();
//             global_count++;
//             sp.unlock();
//         }});
//     }
//     for (auto& thread : threads)
//         thread.join();
//     std::cout << "Global Count: " << global_count << std::endl;

//     return 0;
// }
