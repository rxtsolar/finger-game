#ifndef _GS_TCP_CLIENT_H_
#define _GS_TCP_CLIENT_H_

#include "../tcp-socket.h"

#include <iostream>
#include <sstream>

namespace gs {

using namespace std;

class TCPClient {
public:
	TCPClient(const string& ip) : clientSock(ip, 2564), running(true)
	{
	}

	void run(void)
	{
		string message;
		clientSock.connectSocket();
		while (running) {
			clientSock.recvSocket(message);
			parse(message);
		}
	}

private:
	ClientTCPSocket clientSock;
	bool running;

	void parse(const string& command)
	{
		stringstream ss(command);
		string type;
		string data;
		getline(ss, type, ':');
		getline(ss, data);
		if (type == "MESSAGE") {
			cout << data << endl;
		} else if (type == "ACTION") {
			if (data == "quit") {
				cout << "terminating" << endl;
				running = false;
			}
		}
	}
};

}

#endif
