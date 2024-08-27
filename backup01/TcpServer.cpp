#include "TcpServer.h"
#include "Acceptor.h"
#include "Connection.h"
#include "EventLoop.h"
#include "Socket.h"

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <unistd.h>

TcpServer::TcpServer(std::string ip, uint16_t port, size_t thread_num) {
	this->thread_num = thread_num;
	main_loop = new EventLoop();
	acceptor = new Acceptor(main_loop, ip, port);
	acceptor->set_connect_callback(std::bind(&TcpServer::accept_connection, this, std::placeholders::_1));
	main_loop->set_timeout_callback(std::bind(&TcpServer::epoll_timeout, this, std::placeholders::_1));
	/*
	threadpool = new ThreadPool(thread_num);
	for(int i = 0; i < thread_num; i++) { 
		sub_loops.emplace_back(new EventLoop());
		sub_loops.back()->set_timeout_callback(std::bind(&TcpServer::epoll_timeout, this, std::placeholders::_1));
		threadpool->add_task(std::bind(&EventLoop::loop, sub_loops.back()));
	}
	*/
}

TcpServer::~TcpServer() {
	delete acceptor;
	for(auto& [fd, cn]: connections) {
		close(fd);
	}
}

void TcpServer::start() {
	main_loop->loop();
}

void TcpServer::accept_connection(Socket *client_socket) {
	SPConnection conn(new Connection(main_loop, client_socket));
	//SPConnection conn(new Connection(sub_loops[client_socket->fd() % thread_num], client_socket));
	conn->set_close_callback(std::bind(&TcpServer::connection_close, this, std::placeholders::_1));
	conn->set_error_callback(std::bind(&TcpServer::connection_error, this, std::placeholders::_1));
	conn->set_handle_message_callback(std::bind(&TcpServer::handle_message, this, std::placeholders::_1, std::placeholders::_2));
	conn->set_send_finish_callback(std::bind(&TcpServer::send_finish, this, std::placeholders::_1));
	std::cout << "client socket: " << client_socket->fd() << std::endl;
	connections[client_socket->fd()] = conn;
}

void TcpServer::connection_close(SPConnection connection) {
	std::cout << "client socket: " << connection->get_fd() << " disconnect" << std::endl;
	close(connection->get_fd());
	connections.erase(connection->get_fd());
}

void TcpServer::connection_error(SPConnection connection) {

}

void TcpServer::handle_message(SPConnection connection, std::string& message) {
	//send(connection->get_fd(), message.c_str(), message.size(), 0);
	connection->send(message.c_str(), message.size());
}

void TcpServer::send_finish(SPConnection connection) {

}

void TcpServer::epoll_timeout(EventLoop *loop) {

}

