#ifndef _GS_TCP_SERVER_H_
#define _GS_TCP_SERVER_H_

#include "../tcp-socket.h"

#include <iostream>
#include <thread>
#include <vector>

namespace gs {

using namespace std;

class TCPServer {
public:
	TCPServer(unsigned int n) : maxThreads(n), serverSock(2564, n)
	{
	}

	void accept(void)
	{
		serverSock.acceptSocket(connectSock);
		cout << "connected to " << connectSock.getIP() << endl;
	}

	void loop(void)
	{
		while (1) {
			string message;
			if (connectSock.recvSocket(message)) {
				cout << "got message: " << message << endl;
			} else {
				connectSock.closeSocket();
				cout << "disconnected from " << connectSock.getIP() << endl;
				break;
			}
		}
	}

private:
	ServerTCPSocket serverSock;
	TCPSocket connectSock;
	unsigned int maxThreads;
};

}

#endif
