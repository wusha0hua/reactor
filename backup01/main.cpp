#include <cstdlib>
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

//#include "Channel.h"
#include "TcpServer.h"

using namespace std;

int main(int argc, char *argv[]) {
	if(argc != 2) {
		cout << "usage: ./select port: " << endl;
		return -1;
	}
	int port = atoi(argv[1]);
	TcpServer server("127.0.0.1", port);
	server.start();
}
