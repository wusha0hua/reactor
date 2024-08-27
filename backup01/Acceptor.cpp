#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Epoll.h"
#include "Socket.h"
#include <functional>

Acceptor::Acceptor(EventLoop *loop, std::string& ip, uint16_t port): loop(loop) {
	InternetAddress address = InternetAddress(ip, port);
	server_socket = new Socket(Socket::create_tcp_socket_file_descriptor(), &address);
	server_socket->bind();
	server_socket->listen();

	acceptor_channel = new Channel(server_socket->fd(), loop->get_epoll());
	acceptor_channel->enable_reading();
	acceptor_channel->set_et_mode();
	acceptor_channel->set_read_callback(std::bind(&Acceptor::accept_connection, this));
	loop->get_epoll()->update_channel(acceptor_channel);
}

Acceptor::~Acceptor() {
	delete server_socket;
	delete acceptor_channel;
}

void Acceptor::accept_connection() {
	Socket *client_socket = server_socket->accept(); 
	connect(client_socket);	
}
void Acceptor::set_connect_callback(std::function<void(Socket*)> callback) {
	connect = callback;
}

