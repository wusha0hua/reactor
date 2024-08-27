#pragma once

#include <sys/select.h>
#include <sys/poll.h>
#include <sys/epoll.h>
#include <vector>
#include <unistd.h>

class Channel;
class Epoll {
public:
	Epoll();
	~Epoll();
	void update_channel(Channel *ch);
	void remove_channel(Channel *ch);
	std::vector<Channel*> wait();
private:
	int epoll_fd = -1;
	const static int MAX_EVNETS = 10000;
	epoll_event evs[MAX_EVNETS];
};

