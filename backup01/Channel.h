#pragma once
#include "Socket.h"
#include <cstdint>
#include <sys/epoll.h>
#include <functional>
class Epoll;

class Channel {
public:
	Channel(int fd, Epoll *ep);
	~Channel();
	uint32_t get_fd();
	void set_et_mode();
	void set_in_poll();
	void set_revent(uint32_t revent);
	bool is_in_poll();
	uint32_t get_event();
	uint32_t get_revent();
	void enable_reading();
	void disable_reading();
	void enable_writing();
	void disabel_writing();
	void disabel_all_events();
	void remove();
	void handle_event();
	//void handle_message();
	void set_read_callback(std::function<void()> callback);
	void set_close_callback(std::function<void()> callback);
	void set_error_callback(std::function<void()> callback);
	void set_write_callback(std::function<void()> callback);
private:
	int fd = -1;
	Epoll *ep = nullptr;
	bool in_poll = false;
	uint32_t event;
	uint32_t revent;
	std::function<void()> read_callback;
	std::function<void()> close_callback;
	std::function<void()> error_callback;
	std::function<void()> write_callback;
};

