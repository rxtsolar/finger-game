#ifndef _GS_TCP_SOCKET_H_
#define _GS_TCP_SOCKET_H_

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>

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
		createSocket();
	}

	TCPSocket(int s, const sockaddr_in& a)
	{
		sock = s;
		memcpy(&addr, &a, sizeof(addr));
	}

	~TCPSocket(void)
	{
		closeSocket();
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

	string getIP(void)
	{
		string ip;
		char buffer[INET_ADDRSTRLEN];
		memset(buffer, 0, INET_ADDRSTRLEN);
		ip = inet_ntop(AF_INET, &addr.sin_addr.s_addr, buffer, INET_ADDRSTRLEN);
		return ip;
	}

	void setPort(int port)
	{
		addr.sin_port = htons(port);
	}

	int getPort(void)
	{
		return addr.sin_port;
	}

	void setRecvTimeout(int seconds)
	{
		timeval timeout;
		timeout.tv_sec = seconds;
		timeout.tv_usec = 0;
		if (setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO,
				(char*)&timeout, sizeof(timeout)) < 0) {
			perror("setsockopt");
			exit(-1);
		}
	}

	void setSendTimeout(int seconds)
	{
		timeval timeout;
		timeout.tv_sec = seconds;
		timeout.tv_usec = 0;
		if (setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO,
				(char*)&timeout, sizeof(timeout)) < 0) {
			perror("setsockopt");
			exit(-1);
		}
	}

	int sendSocket(const char* data, int len) const
	{
		return send(sock, data, len, 0);
	}

	bool sendSocket(const string& data) const
	{
		if (sendSocket(data.c_str(), data.size()) < 0)
			return false;
		return true;
	}

	int recvSocket(char* data, int len) const
	{
		return recv(sock, data, len, 0);
	}

	bool recvSocket(string& data) const
	{
		char buffer[2048];
		memset(buffer, 0, 2048);
		int ret = recvSocket(buffer, 2048);
		cerr << ret << endl;
		if (ret <= 0) {
		/*if (recvSocket(buffer, 2048) <= 0)*/
			perror("recv");
			return false;
		}
		data = string(buffer);
		return true;
	}

	void createSocket(void)
	{
		addr.sin_family = AF_INET;
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock < 0) {
			perror("socket");
			exit(-1);
		}
	}

	void closeSocket(void)
	{
		if (close(sock) < 0) {
			perror("close");
			exit(-1);
		}
	}

	int sock;
	sockaddr_in addr;
};

class ServerTCPSocket : public TCPSocket {
public:
	ServerTCPSocket(int port, int listeners)
	{
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
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

	TCPSocket* acceptSocket(void)
	{
		TCPSocket* connectSock;
		sockaddr_in a;
		socklen_t len = sizeof(a);
		int s = accept(sock, (sockaddr*)&a, &len);
		if (s < 0) {
			perror("accept");
			exit(-1);
		}
		connectSock = new TCPSocket(s, a);
		return connectSock;
	}
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
};

}

#endif
