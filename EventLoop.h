#pragma once

#include <functional>
#include <memory>
#include "Epoll.h"

class Epoll;

class EventLoop {
public:
	EventLoop();
	~EventLoop();
	void loop();
	const std::unique_ptr<Epoll>& get_epoll();
	void set_timeout_callback(std::function<void(EventLoop*)> callback);
private:
	std::unique_ptr<Epoll> ep;
	std::function<void(EventLoop*)> epoll_timeout_callback;
};

