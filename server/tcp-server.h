#ifndef _GS_TCP_SERVER_H_
#define _GS_TCP_SERVER_H_

#include "tcp-socket.h"

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
	}

	void loop(void)
	{
		while (1) {
			cout << serverSock.getMessage() << endl;
		}
	}

private:
	ServerTCPSocket serverSock;
};

}

#endif
