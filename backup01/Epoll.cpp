#include "Epoll.h"
#include "Channel.h"

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ostream>
#include <sys/epoll.h>
#include <vector>

Epoll::Epoll() {
	if((epoll_fd = epoll_create(1)) <= 0) {
		exit(-1);
	}
}

Epoll::~Epoll() {
	if(epoll_fd >= 0) close(epoll_fd);
}

void Epoll::update_channel(Channel *ch) {
	epoll_event ev;
	ev.data.ptr = ch;
	ev.events = ch->get_event();
	if(ch->is_in_poll()) {
		if(epoll_ctl(epoll_fd, EPOLL_CTL_MOD, ch->get_fd(), &ev) == -1) {
			//////////////////////////
			exit(-1);
		} 
	} else {
		if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ch->get_fd(), &ev) == -1) {
			std::cout << strerror(errno) << std::endl;

			///////////
			exit(-1);
		}	
		ch->set_in_poll();
	}
}

std::vector<Channel*> Epoll::wait() {
	std::vector<Channel*> chs;
	int count = epoll_wait(epoll_fd, evs, MAX_EVNETS, -1);
	if(count < 0) {
		////////////////////////
		exit(-1);
	}
	if(count == 0) {
		/////////////////
		return chs;
	}
	for(int i = 0; i < count; i++) {
		Channel *ch = (Channel*)evs[i].data.ptr;
		ch->set_revent(evs[i].events);
		chs.emplace_back(ch);
	}
	return chs;
}

void Epoll::remove_channel(Channel *ch) {
	if(ch->is_in_poll()) {
		if((epoll_ctl(epoll_fd, EPOLL_CTL_DEL, ch->get_fd(), 0)) == -1) {
			/////////////////////////
		}
	}
}

