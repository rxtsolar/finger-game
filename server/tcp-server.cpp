#include "tcp-server.h"

using namespace gs;

int main(void)
{
	TCPServer server;
	server.serve();
	return 0;
}
