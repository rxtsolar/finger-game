#ifndef _GS_TCP_SOCKET_H_
#define _GS_TCP_SOCKET_H_

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

using namespace std;

class TCPSocket {
public:
	TCPSocket(void)
	{
		addr.sin_family = AF_INET;
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0) {
			perror("socket");
			exit(-1);
		}
	}

	TCPSocket(int fd)
	{
		sock = fd;
		addr.sin_family = AF_INET;
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0) {
			perror("socket");
			exit(-1);
		}
	}

	void setSocket(int fd)
	{
		sock = fd;
	}

	int getSocket(void) const
	{
		return sock;
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

class ServerTCPSocket : public TCPSocket {
public:
	ServerTCPSocket(int port, int listeners)
	{
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		acceptLen = sizeof(acceptAddr);
		setPort(port);

		if (bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
			perror("bind");
			exit(-1);
		}

		if (listen(sock, listeners)) {
			perror("listen");
			exit(-1);
		}
	}

	void acceptSocket(void)
	{
		int s = accept(sock, (sockaddr*)&acceptAddr, &acceptLen);
		if (s < 0) {
			perror("accept");
			exit(-1);
		}
		connectSock.setSocket(s);
	}

	void sendMessage(const string& message) const
	{
		connectSock.sendSocket(message);
	}

	string getMessage(void) const
	{
		string message;
		connectSock.recvSocket(message);
		return message;
	}

protected:
	TCPSocket connectSock;
	sockaddr_in acceptAddr;
	socklen_t acceptLen;
};

class ClientTCPSocket : public TCPSocket {
public:
	ClientTCPSocket(const string& ip, int port)
	{
		setIP(ip);
		setPort(port);
	}

	void connectSocket(void)
	{
		if (connect(sock, (sockaddr*)&addr, sizeof(addr)) < 0) {
			perror("connect");
			exit(-1);
		}
	}

	void sendMessage(const string& message)
	{
		sendSocket(message);
	}

	string getMessage(void)
	{
		string message;
		recvSocket(message);
		return message;
	}
};

}

#endif
