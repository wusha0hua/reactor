#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include <memory>

EventLoop::EventLoop(): ep(new Epoll()) {
}

EventLoop::~EventLoop() {
}

void EventLoop::loop() {
	while(true) {
		std::vector<Channel*> channels = ep->wait();
		if(channels.empty()) {
			epoll_timeout_callback(this);
		}
		for(auto ch: channels) {
			ch->handle_event();
		}
	}
}

const std::unique_ptr<Epoll>& EventLoop::get_epoll() {
	return ep;
}
	
void EventLoop::set_timeout_callback(std::function<void(EventLoop*)> callback) {
	epoll_timeout_callback = callback;
}

