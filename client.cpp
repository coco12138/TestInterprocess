#include "client.h"

void client::writeToMemory(const char * file_path)
{
	
	cout << "writeToMemory start..." << endl;
	FILE *stream_wr;
	errno_t er = fopen_s(&stream_wr, file_path, "rb");
	if (er)
		return;
	boost::interprocess::named_mutex named_mtx(boost::interprocess::open_only, "mtx");
	boost::interprocess::named_mutex named_mtx2(boost::interprocess::open_or_create, "mtx2");
	//boost::interprocess::named_condition named_cnd(boost::interprocess::open_only, "cnd");
	//boost::interprocess::scoped_lock<boost::interprocess::named_mutex> lock(named_mtx);
	named_mtx2.lock();
	while (fread(mem_buffer_, sizeof(char), size_, stream_wr))
	{
		
		strcpy_s(address_, strlen(mem_buffer_) + 1, mem_buffer_);
		named_mtx.unlock();

		memset(mem_buffer_, 0, size_ + 1);
		named_mtx2.lock();
		
	}
	named_mtx.unlock();
	fclose(stream_wr);
	cout << "writeToMemory end..." << endl;
	
}
