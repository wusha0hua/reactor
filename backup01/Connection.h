#pragma once

#include "Socket.h"
#include "Buffer.h"
#include <atomic>
#include <functional>
#include <string>
#include <memory>

class Connection;
using SPConnecction = std::shared_ptr<Connection>;

class EventLoop;
class Channel;

class Connection: public std::enable_shared_from_this<Connection> {
public:
	Connection(EventLoop *loop, Socket *client_socket);
	~Connection();
	void connection_close();
	void connection_error();
	int get_fd();
	void set_close_callback(std::function<void(SPConnecction)> callback);
	void set_error_callback(std::function<void(SPConnecction)> callback);
	void set_handle_message_callback(std::function<void(SPConnecction, std::string&)> callback);
	void set_send_finish_callback(std::function<void(SPConnecction)> callback);
	void read_message();
	void write_message();
	void send(const char *data, size_t len);
private:
	EventLoop *loop;
	Socket *client_socket;
	Channel* connection_channel;
	std::function<void(SPConnecction)> close_callback;
	std::function<void(SPConnecction)> error_callback;
	std::function<void(SPConnecction, std::string&)> handle_message_callback;
	std::function<void(SPConnecction)> send_finish_callback;
	Buffer input;
	Buffer output;
	std::atomic<bool> is_disconnect;
};
