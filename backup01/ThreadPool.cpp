#include "ThreadPool.h"	
#include <mutex>
#include <thread>

ThreadPool::ThreadPool(size_t thread_num) {
	quit = false;
	if(thread_num <= 0 || thread_num > std::thread::hardware_concurrency()) 
		thread_num = std::thread::hardware_concurrency();
	auto func = [this]() {
		while(!quit) {
			std::unique_lock<std::mutex> lock(mutex);
			condition.wait(lock, [this]() {return quit || !task_queue.empty();});
			if(task_queue.empty()) continue;
			auto task = std::move(task_queue.front());
			task_queue.pop();
			lock.unlock();
			task();
		}
	};
	for(int i = 0; i < thread_num; i++) {
		threads.emplace_back(func);
	}
}
	
ThreadPool::~ThreadPool() {
	quit = true;
	condition.notify_all();
	for(int i = 0; i < threads.size(); i++)
		if(threads[i].joinable())
			threads[i].join();
}
	
void ThreadPool::add_task(std::function<void()> task) {
	std::unique_lock<std::mutex> lock(mutex);
	task_queue.push(task);
	condition.notify_one();
}

