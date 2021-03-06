#ifndef _GS_TCP_SERVER_H_
#define _GS_TCP_SERVER_H_

#include "../tcp-socket.h"
#include "game.h"

#include <iostream>
#include <mutex>
#include <thread>
#include <set>

namespace gs {

using namespace std;

class TCPServer {
public:
	TCPServer(void) : serverSock(2564, 1), waitingSock(0)
	{
	}

	void run(void)
	{
		while (true)
			accept();
	}

private:
	ServerTCPSocket serverSock;
	TCPSocket* waitingSock;
	set<TCPSocket*> connectedSocks;
	mutex lock;

	void accept(void)
	{
		TCPSocket* sock;
		sock = serverSock.acceptSocket();
		lock.lock();
		connectedSocks.insert(sock);
		lock.unlock();
		cout << "connected to " << sock->getIP() << endl;

		if (!waitingSock) {
			waitingSock = sock;
			sock->sendSocket("Waiting for another player to join in");
		} else {
			cout << sock->getIP() << " and " << waitingSock->getIP()
				<< " paired" << endl;
			thread t(&TCPServer::start, this, waitingSock, sock);
			t.detach();
			waitingSock = 0;
		}
	}

	void start(TCPSocket* sock0, TCPSocket* sock1)
	{
		Game game(sock0, sock1);

		game.run();

		lock.lock();
		connectedSocks.erase(sock0);
		connectedSocks.erase(sock1);
		delete sock0;
		delete sock1;
		lock.unlock();

		cout << "disconnected from " << sock0->getIP() << " and "
			<< sock1->getIP() << endl;
	}
};

}

#endif
