#include "tcp-server.h"

using namespace gs;

int main(void)
{
	TCPServer server;
	while (1) {
		server.accept();
		server.loop();
	}
	return 0;
}
