#include "tcp-client.h"

using namespace gs;

int main(int argc, char* argv[])
{
	TCPClient client(argv[1]);
	client.run();
	return 0;
}
