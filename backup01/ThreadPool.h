#include <memory>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>
#include <queue>
#include <vector>

class ThreadPool {
public:
	ThreadPool(size_t thread_num = 0);
	~ThreadPool();
	void add_task(std::function<void()> task);
private:
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> task_queue;
	std::mutex mutex;
	std::condition_variable condition;
	std::atomic<bool> quit;
};
