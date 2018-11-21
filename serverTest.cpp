#include "server.h"


void main()
{
	boost::interprocess::shared_memory_object::remove("my_server");
	server s("my_server", 1024);
	s.save<char>("content");

	system("pause");
}
