#pragma once

#include "InternetAdress.h"
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>


class Socket {
public:
	Socket(int fd, InternetAddress* address);
	~Socket();
	static int create_tcp_socket_file_descriptor();
	bool bind();
	bool listen();
	void set_non_block(bool op);
	void set_reuse_addr(bool op);
	void set_reuse_port(bool op);
	void set_tcp_nodelay(bool op);
	void set_keep_alive(bool op);
	int fd();
	Socket *accept();
	void set_address(InternetAddress *address);
	const InternetAddress* get_address();
private:
	int sockfd;
	InternetAddress address;
};
