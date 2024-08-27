#include "InternetAdress.h"
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
InternetAddress::InternetAddress(std::string ip, unsigned short port, unsigned short family) {
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = inet_addr(ip.c_str());
	addr.sin_port = htons(port);
	addr.sin_family = family;
}

InternetAddress::InternetAddress(sockaddr* address) {
	memcpy(&addr, address, sizeof(addr));
}

std::string InternetAddress::ip() {
	return std::string(inet_ntoa(addr.sin_addr));
}
unsigned int InternetAddress::port() {
	return ntohs(addr.sin_port);
}
const sockaddr* InternetAddress::address() {
	return (const sockaddr*)&addr;
}	

void InternetAddress::set_address(const sockaddr *addr_in) {
	memcpy(&addr, addr_in, sizeof(sockaddr_in));
}

