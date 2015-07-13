#ifndef _GS_TCP_CLIENT_H_
#define _GS_TCP_CLIENT_H_

#include "tcp-socket.h"

#include <iostream>

namespace gs {

using namespace std;

class TCPClient {
public:
	TCPClient(const string& ip) : clientSock(ip, 2564)
	{
	}

	void connect(void)
	{
		clientSock.connectSocket();
	}

	void loop(void)
	{
		string message;
		while (1) {
			cin >> message;
			clientSock.sendMessage(message);
		}
	}

private:
	ClientTCPSocket clientSock;
};

}

#endif
