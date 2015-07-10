#ifndef _GS_SOCKET_H_
#define _GS_SOCKET_H_

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>

#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>

namespace gs {

class TCPSocket {
public:
	TCPSocket(void)
	{
		addr.sin_family = AF_INET;
	}

	TCPSocket(int fd)
	{
		sock = fd;
		addr.sin_family = AF_INET;
	}

	void setSocket(int fd)
	{
		sock = fd;
	}

	int getSocket(void) const
	{
		return sock;
	}

	void createSocket(void)
	{
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0) {
			perror("socket");
			exit(-1);
		}
	}

	bool setIP(const string& ip)
	{
		hostent* hostName = gethostbyname(ip.c_str());
		if (!hostName) {
			perror("gethostbyname");
			return false;
		}
		addr.sin_addr.s_addr = *(u_long*)hostName->h_addr_list[0];
		return true;
	}

	void setPort(int port)
	{
		addr.sin_port = htons(port);
	}

	int sendSocket(const char* data, int len) const
	{
		return send(sock, data, len, 0);
	}

	int sendSocket(const string& data) const
	{
		return sendSocket(data.c_str(), data.size());
	}

	int recvSocket(char* data, int len) const
	{
		return recv(sock, data, len, 0);
	}

	int recvSocket(string& data) const
	{
		char buffer[2048];
		int ret = recvSocket(buffer, 2048);
		data = string(buffer);
		return ret;
	}

	int closeSocket(void)
	{
		return close(sock);
	}

	int operator == (const TCPSocket& s) const
	{
		return sock == s.getSocket();
	}

protected:
	int sock;
	sockaddr_in addr;
};

class ListenTCPSocket : public TCPSocket {
public:
	ListenTCPSocket(void)
	{
		createSocket();
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		acceptLen = sizeof(acceptAddr);
	}

	int bindSocket(void) const
	{
		return bind(sock, (sockaddr*)&addr, sizeof(addr));
	}

	int listenSocket(int n) const
	{
		return listen(sock, n);
	}

	int acceptSocket(void)
	{
		return accept(sock, (sockaddr*)&acceptAddr, &acceptLen);
	}

protected:
	sockaddr_in acceptAddr;
	socklen_t acceptLen;
};

}

#endif
