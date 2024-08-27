#pragma once

#include <functional>

class Epoll;

class EventLoop {
public:
	EventLoop();
	~EventLoop();
	void loop();
	Epoll *get_epoll();
	void set_timeout_callback(std::function<void(EventLoop*)> callback);
private:
	Epoll *ep;
	std::function<void(EventLoop*)> epoll_timeout_callback;
};

