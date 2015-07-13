#include "tcp-client.h"

using namespace gs;

int main(int argc, char* argv[])
{
	TCPClient client(argv[1]);
	while (1) {
		client.connect();
		client.loop();
	}
	return 0;
}
