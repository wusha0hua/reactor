#pragma once
#include "Socket.h"
#include "Channel.h"

#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>

class EventLoop;

class Acceptor {
public:
	Acceptor(const std::unique_ptr<EventLoop>& loop, std::string& ip, uint16_t port);
	~Acceptor();
	void accept_connection();
	void set_connect_callback(std::function<void(std::unique_ptr<Socket>)> callback);
private:
	const std::unique_ptr<EventLoop>& loop;
	Socket server_socket;
	Channel acceptor_channel;
	std::function<void(std::unique_ptr<Socket>)> connect;
};
