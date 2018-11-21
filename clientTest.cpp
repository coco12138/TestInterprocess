#include "client.h"


void main()
{
	client s("my_server", 1024);
	s.init_memory<char>();
	s.writeToMemory("test.txt");

	system("pause");
}
