#include "Socket.h"
#include "InternetAdress.h"
#include <cstring>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>

	
Socket::Socket(int fd) {
	sockfd = fd;
}

Socket::~Socket() {
}

int Socket::create_tcp_socket_file_descriptor() {
	int socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	
	return socketfd;
}

bool Socket::bind(InternetAddress* in_addr) {
	address.set_address(in_addr->address());		
	if(::bind(sockfd, address.address(), sizeof(struct sockaddr_in)) < 0) {
		std::cout << "bind fail" << std::endl;
		close(sockfd);
		exit(-1);
		return false;
	}
	return true;
}

bool Socket::listen() {
	if(::listen(sockfd, 5) != 0) {
		std::cout << "listen fail" << std::endl;
		close(sockfd);
		exit(-1);
		return false;
	}
	return true;
}

void Socket::set_non_block(bool op) {
	int flag = fcntl(sockfd, F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(sockfd, F_SETFL, flag);
}

void Socket::set_reuse_addr(bool op) {
	int opval = op? 1: 0;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opval, sizeof(opval));
}

void Socket::set_reuse_port(bool op) {
	int opval = op? 1: 0;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opval, sizeof(opval));
}

void Socket::set_tcp_nodelay(bool op) {
	int opval = op? 1: 0;
	setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &opval, sizeof(opval));
}

void Socket::set_keep_alive(bool op) {
	int opval = op? 1: 0;
	setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &opval, sizeof(opval));
}

int Socket::fd() {
	return sockfd;
}

Socket* Socket::accept() {
	sockaddr addr;
	memset(&addr, 0, sizeof(sockaddr));
	socklen_t len = sizeof(sockaddr);
	int new_sockfd = ::accept(sockfd, &addr, &len);	
	if(new_sockfd < 0) {
		//////////////////////////
		exit(-1);
	}
	InternetAddress in_addr(&addr);
	Socket *new_sock = new Socket(new_sockfd);
	new_sock->set_address(&in_addr);
	return new_sock;
}

void Socket::set_address(InternetAddress *address) {
	//memcpy(&this->address, address, sizeof(InternetAddress));
	address->set_address(address->address());
}

const InternetAddress* Socket::get_address() {
	return &this->address;
}

