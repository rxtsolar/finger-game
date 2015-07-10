#ifndef _GS_EPOLL_H_
#define _GS_EPOLL_H_

#include "gs_socket.h"

#include <cstring>

#include <fcntl.h>
#include <sys/epoll.h>

namespace gs {

struct Data {
	int fd;
	unsigned int len;
	char buf[2048];
	Data() { }
	Data(const char* s, const int l)
	{
		memcpy(buf, s, l);
	}
};

class EpollManager {
public:
	EpollManager(int n)
	{
		fd = epoll_create(1);
		if (fd < 0) {
			perror("epoll_create");
			exit(-1);
		}

		maxEvents = n;
		timeout = -1;
	}

	int addSocket(int sock)
	{
		epoll_event e;
		e.events = EPOLLIN | EPOLLET;
		e.data.fd = sock;
		fcntl(sock, F_SETFL, O_NONBLOCK);
		return epoll_ctl(fd, EPOLL_CTL_ADD, sock, &e);
	}

	void modIn(int index)
	{
		epoll_event e;
		e.data.fd = *(int*)events[index].data.ptr;
		e.events = EPOLLIN | EPOLLET;
		epoll_ctl(fd, EPOLL_CTL_MOD, *(int*)events[index].data.ptr, &e);
	}

	void modOut(int index, Data* data)
	{
		epoll_event e;
		data->fd = events[index].data.fd;
		e.data.ptr = data;
		e.events = EPOLLOUT | EPOLLET;
		epoll_ctl(fd, EPOLL_CTL_MOD, events[index].data.fd, &e);
	}

	int wait(void)
	{
		return epoll_wait(fd, events, maxEvents, timeout);
	}

	TCPSocket getSocket(int index)
	{
		return TCPSocket(events[index].data.fd);
	}

	TCPSocket getSocket(Data* data)
	{
		return TCPSocket(data->fd);
	}

	Data* getData(int index)
	{
		return (Data*)events[index].data.ptr;
	}

private:
	int fd;
	int maxEvents;
	int timeout;
	epoll_event* events;
};

}

#endif
