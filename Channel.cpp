#include "Channel.h"

#include "Epoll.h"
#include <iostream>
#include <cstring>
#include <memory>
#include <sys/epoll.h>

/*
Channel::Channel(): ep(nullptr) {
	
}
*/

Channel::Channel(int fd, const std::unique_ptr<Epoll>& ep): fd(fd), ep(ep) {	
}

Channel::~Channel() {

}

uint32_t Channel::get_fd() {
	return fd;
}

void Channel::set_et_mode() {
	event |= EPOLLET;
}

void Channel::set_in_poll() {
	in_poll = true;
}

void Channel::set_revent(uint32_t revent) {
	this->revent = revent;
}

bool Channel::is_in_poll() {
	return in_poll;
}

uint32_t Channel::get_event() {
	return event;
}

uint32_t Channel::get_revent() {
	return revent;
}

void Channel::enable_reading() {
	event |= EPOLLIN;
	ep->update_channel(this);
}

void Channel::enable_writing() {
	event |= EPOLLOUT;
	ep->update_channel(this);
}

void Channel::disable_reading() {
	event &= ~EPOLLIN;
	ep->update_channel(this);
}
	
void Channel::disabel_writing() {
	event &= ~EPOLLOUT;
	ep->update_channel(this);
}

void Channel::handle_event() {
	if(revent & EPOLLRDHUP)	{	// close
		//remove();
		close_callback();	
	} else if(revent & (EPOLLIN | EPOLLPRI)) {
		read_callback();
	} else if(revent & EPOLLOUT) {
		write_callback();		
	} else {
		//remove();
		error_callback();
	}
}

/*
void Channel::handle_connction(Socket& server_socket) {
	Socket *client_socket = server_socket.accept(); 
	std::cout << "client socket: " << client_socket->fd() << std::endl;

	Channel *connection_channel = new Channel(client_socket->fd(), ep);
	connection_channel->enable_reading();
	connection_channel->set_et_mode();
	connection_channel->set_read_callback(std::bind(&Channel::handle_message, connection_channel));
	ep->update_channel(connection_channel);

}
*/

/*
void Channel::handle_message() {
	std::string buffer;
	char buf[11];
	memset(buf, 0, 11);
	if(recv(fd, buf, 10, 0) <= 0) {
		close_callback();
	} else {
		buffer = std::string(buf);
		std::cout << "recv: " << buffer << std::endl;
		send(fd, buffer.data(), buffer.size(), 0);
	}	
}
*/
	
void Channel::set_read_callback(std::function<void()> callback) {
	read_callback = callback;
}
	
void Channel::set_close_callback(std::function<void()> callback) {
	close_callback = callback;
}
	
void Channel::set_error_callback(std::function<void()> callback) {
	error_callback = callback;
}

void Channel::set_write_callback(std::function<void()> callback) {
	write_callback = callback;
}
	
void Channel::disabel_all_events() {
	event = 0;
	ep->update_channel(this);
}
	
void Channel::remove() {
	disabel_all_events();
	ep->remove_channel(this);
}

