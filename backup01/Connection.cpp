#include "Connection.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Epoll.h"

#include <cerrno>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>

Connection::Connection(EventLoop *loop, Socket *client_socket): loop(loop), client_socket(client_socket) {
	is_disconnect = false;
	connection_channel = new Channel(client_socket->fd(), loop->get_epoll());
	connection_channel->enable_reading();
	connection_channel->set_et_mode();
	connection_channel->set_read_callback(std::bind(&Connection::read_message, this));
	connection_channel->set_close_callback(std::bind(&Connection::connection_close, this));
	connection_channel->set_error_callback(std::bind(&Connection::connection_error, this));
	connection_channel->set_write_callback(std::bind(&Connection::write_message, this));
	loop->get_epoll()->update_channel(connection_channel);
}

Connection::~Connection() {
	//delete connection_channel;
	delete client_socket;
}

int Connection::get_fd() {
	return client_socket->fd();
}

void Connection::connection_close() {
	is_disconnect = true;
	connection_channel->remove();
	close_callback(shared_from_this());
}

void Connection::connection_error() {
	is_disconnect = true;
	connection_channel->remove();
	error_callback(shared_from_this());
}

void Connection::set_close_callback(std::function<void(SPConnecction)> callback) {
	close_callback = callback;
}
	
void Connection::set_error_callback(std::function<void(SPConnecction)> callback) {
	error_callback = callback;
}

void Connection::read_message() {
	char buff[1025];
	while(true) {
		memset(buff, 0, sizeof(buff));
		size_t len = read(client_socket->fd(), buff, sizeof(buff) - 1);
		if(len == sizeof(buff) - 1) {
			input.append(buff, len);
		} else if(len > 0) { 
			input.append(buff, len);
			std::cout << "recv: " << input.data();
			//send(client_socket->fd(), output.data().c_str(), output.size(), 0);
			handle_message_callback(shared_from_this(), input.data());
			input.clear();
			//output.clear();
			break;
		}else if(len == -1 && errno == EINTR) { // interrupt and continue
			continue;
		} else if(len == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) { // read finish
			std::cout << "recv: " << input.data();
			output = input;
			break;
		} else if(len == 0) { // disconnect
			//connection_channel->remove();
			this->close_callback(shared_from_this());
			break;
		}
	}
}

void Connection::write_message() {
	int len = write(client_socket->fd(), output.data().c_str(), output.size());
	if(len > 0) output.erase(0, len);
	if(output.size() == 0) {
		send_finish_callback(shared_from_this());
		connection_channel->disabel_writing();
	}
}

void Connection::set_handle_message_callback(std::function<void(SPConnecction, std::string&)> callback) {
	handle_message_callback = callback;
}
	
void Connection::send(const char *data, size_t len) {
	if(is_disconnect) {
		return;
	}
	output.append(data, len);
	connection_channel->enable_writing();
}

void Connection::set_send_finish_callback(std::function<void(SPConnecction)> callback) {
	send_finish_callback = callback;
}

