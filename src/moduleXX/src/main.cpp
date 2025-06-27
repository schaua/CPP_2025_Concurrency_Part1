#include <thread>
#include <iostream>
#include <mutex>

std::mutex mut{};

int tier3(){
    std::unique_lock lock{mut};
    std::cout << "Tier 3" << std::endl;
    return 42;
}

int tier2() {
    std::cout << "Tier 2" << std::endl;
    return tier3();
}

int tier1() {
    std::unique_lock lock{mut};
    std::cout << "Tier 1" << std::endl;
    return tier2();
}

int main(int argc, char const *argv[])
{
    std::thread t1{tier1};
    std::thread t2{tier2};

    t1.join();
    t2.join();
    return 0;
}

