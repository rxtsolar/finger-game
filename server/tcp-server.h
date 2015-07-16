#ifndef _GS_TCP_SERVER_H_
#define _GS_TCP_SERVER_H_

#include "../tcp-socket.h"

#include <iostream>
#include <mutex>
#include <thread>
#include <set>

namespace gs {

using namespace std;

class TCPServer {
public:
	TCPServer(void) : serverSock(2564, 1)
	{
	}

	void serve(void)
	{
		while (true)
			accept();
	}

private:
	ServerTCPSocket serverSock;
	set<TCPSocket*> connectedSocks;
	mutex lock;

	void accept(void)
	{
		TCPSocket* sock;
		lock.lock();
		sock = serverSock.acceptSocket();
		connectedSocks.insert(sock);
		lock.unlock();
		cout << "connected to " << sock->getIP() << endl;

		thread t(&TCPServer::loop, this, sock);
		t.detach();
	}

	void loop(TCPSocket* sock)
	{
		while (1) {
			string message;
			if (sock->recvSocket(message)) {
				cout << "got message: " << message << endl;
			} else {
				cout << "disconnected from " << sock->getIP() << endl;
				lock.lock();
				connectedSocks.erase(sock);
				delete sock;
				lock.unlock();
				break;
			}
		}
	}
};

}

#endif
