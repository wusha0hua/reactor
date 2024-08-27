#pragma once

#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

class InternetAddress {
public:
	InternetAddress() {}
	InternetAddress(std::string ip, unsigned short port, unsigned short family = AF_INET);
	InternetAddress(sockaddr* address);
	std::string ip();
	unsigned int port();
	const sockaddr* address();
private:
	sockaddr_in addr;
};
