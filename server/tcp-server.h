#ifndef _GS_TCP_SERVER_H_
#define _GS_TCP_SERVER_H_

#include "../tcp-socket.h"

#include <iostream>
#include <thread>

namespace gs {

using namespace std;

class TCPServer {
public:
	TCPServer(void) : serverSock(2564, 1)
	{
	}

	void accept(void)
	{
		serverSock.acceptSocket(connectSock);
		cout << "connected" << endl;
	}

	void loop(void)
	{
		while (1) {
			string message;
			if (connectSock.recvSocket(message)) {
				cout << "got message: " << message << endl;
			} else {
				connectSock.closeSocket();
				cout << "disconnected" << endl;
				break;
			}
		}
	}

private:
	ServerTCPSocket serverSock;
	TCPSocket connectSock;
};

}

#endif
