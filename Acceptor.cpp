#include "Acceptor.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Epoll.h"
#include "Socket.h"
#include <functional>
#include <memory>

Acceptor::Acceptor(const std::unique_ptr<EventLoop>& loop, std::string& ip, uint16_t port): loop(loop), server_socket(Socket(Socket::create_tcp_socket_file_descriptor())), acceptor_channel(Channel(server_socket.fd(), loop->get_epoll())) {
	InternetAddress address = InternetAddress(ip, port);
	//server_socket = Socket(Socket::create_tcp_socket_file_descriptor(), &address);
	server_socket.bind(&address);
	server_socket.listen();

	//acceptor_channel = std::move(Channel(server_socket.fd(), loop->get_epoll()));
	acceptor_channel.enable_reading();
	acceptor_channel.set_et_mode();
	acceptor_channel.set_read_callback(std::bind(&Acceptor::accept_connection, this));
}

Acceptor::~Acceptor() {
}

void Acceptor::accept_connection() {
	std::unique_ptr<Socket> client_socket(server_socket.accept()); 
	connect(std::move(client_socket));	
}
void Acceptor::set_connect_callback(std::function<void(std::unique_ptr<Socket>)> callback) {
	connect = callback;
}

