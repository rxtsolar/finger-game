#ifndef _GS_TCP_CLIENT_H_
#define _GS_TCP_CLIENT_H_

#include "../tcp-socket.h"
#include "game.h"

#include <iostream>
#include <sstream>

namespace gs {

using namespace std;

class TCPClient {
public:
	TCPClient(const string& ip) : clientSock(ip, 2564)
	{
		clientSock.connectSocket();
	}

	void run(void)
	{
		Game game(&clientSock);

		game.run();
	}

private:
	ClientTCPSocket clientSock;
};

}

#endif
