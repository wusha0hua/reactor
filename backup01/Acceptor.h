#pragma once
#include "Socket.h"
#include <cstdint>
#include <functional>
#include <iostream>

class EventLoop;
class Channel;

class Acceptor {
public:
	Acceptor(EventLoop *loop, std::string& ip, uint16_t port);
	~Acceptor();
	void accept_connection();
	void set_connect_callback(std::function<void(Socket*)> callback);
private:
	EventLoop *loop;
	Socket *server_socket;
	Channel *acceptor_channel;
	std::function<void(Socket*)> connect;
};
