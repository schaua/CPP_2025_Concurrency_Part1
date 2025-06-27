#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>
#include <iostream>

class ThreadPool {
public:
    ThreadPool(int numThreads) : numThreads_(numThreads){
        for (int i = 0; i < numThreads; ++i){
            threads_.emplace_back([this]{
                while(true){
                    std::unique_lock<std::mutex> lock(queueMutex_);
                    cv_.wait(lock, [this] {return !tasks_.empty() || shouldStop_;});
                    if (shouldStop_ && tasks_.empty()){
                        return;
                    }
                    auto task = std::move(tasks_.front());
                    tasks_.pop();
                    lock.unlock();
                    task();
                }
            });
        }      
    }
    ~ThreadPool() {
        shouldStop_ = true;
        cv_.notify_all();
        for (auto& thread : threads_){
            thread.join();
        }
    }

    template<typename F, typename... Args>
    void enqueue(F&& f, Args&&... args){
        {        
            std::unique_lock<std::mutex> lock(queueMutex_);
            tasks_.emplace([=](){
                f(std::forward<Args>(args)...);
            });
        }
        cv_.notify_one();
    }


private:
    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex queueMutex_;
    std::condition_variable cv_;
    int numThreads_;
    bool shouldStop_ = false;

};

int main(int argc, char const *argv[])
{
    std::mutex outMut;
    ThreadPool ourPool{4};

    ourPool.enqueue([&]{
        std::unique_lock<std::mutex> lock(outMut);
        std::cout << "First task\n";
    });
    ourPool.enqueue([&]{
        std::unique_lock<std::mutex> lock(outMut);
        std::cout << "Second task" << std::endl;
    });
    int x = 42, y = 2, z = 0;
    ourPool.enqueue([&]{
        std::unique_lock<std::mutex> lock(outMut);
        std::cout << "Sum: " << x + y + z << std::endl;
    });


    return 0;
}
