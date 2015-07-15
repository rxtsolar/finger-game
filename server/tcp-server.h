#ifndef _GS_TCP_SERVER_H_
#define _GS_TCP_SERVER_H_

#include "../tcp-socket.h"

#include <iostream>

namespace gs {

using namespace std;

class TCPServer {
public:
	TCPServer(void) : serverSock(2564, 1)
	{
	}

	void accept(void)
	{
		serverSock.acceptSocket();
		cout << "connected" << endl;
	}

	void loop(void)
	{
		while (1) {
			string message;
			if (serverSock.getMessage(message)) {
				cout << "got message: " << message << endl;
			} else {
				cout << "disconnected" << endl;
				break;
			}
		}
	}

private:
	ServerTCPSocket serverSock;
};

}

#endif
