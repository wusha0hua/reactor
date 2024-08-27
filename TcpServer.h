#pragma once
#include "ThreadPool.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include <iostream>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <vector>

class Acceptor;
class Socket;
class Connection;
using SPConnection = std::shared_ptr<Connection>;

class TcpServer {
public:
	TcpServer(std::string ip, uint16_t port, size_t thread_num = std::thread::hardware_concurrency());
	~TcpServer();
	void start();
	void accept_connection(std::unique_ptr<Socket> client_socket);
	void connection_close(SPConnection connection);
	void connection_error(SPConnection connection);
	void handle_message(SPConnection connection, std::string& message);
	void send_finish(SPConnection connection);
	void epoll_timeout(EventLoop *loop);
private:
	std::unique_ptr<EventLoop> main_loop;
	std::vector<std::unique_ptr<EventLoop>> sub_loops;
	ThreadPool threadpool;
	size_t thread_num;
	Acceptor acceptor;
	std::unordered_map<int, SPConnection> connections;
};
