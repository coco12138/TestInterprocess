#ifndef SERVER_H_
#define SERVER_H_

#include <boost\interprocess\managed_shared_memory.hpp>
#include <iostream>
#include <boost\thread.hpp>
#include <boost\interprocess\sync\named_condition.hpp>
#include <boost\interprocess\sync\named_mutex.hpp>
#include <fstream>
using namespace std;

class server
{
public:
	server(const char * shared_name, size_t size) 
		:managed_shd(boost::interprocess::open_or_create, shared_name,size),
		size_(size),
		offset_(0)
	{
		shared_name_ = new char[strlen(shared_name) + 1];
		memset(shared_name_, 0, strlen(shared_name) + 1);
		strcpy_s(shared_name_, strlen(shared_name) + 1, shared_name);
	}
	void setFindName(const char *find_name)
	{
		find_name_ = new char[strlen(find_name) + 1];
		memset(find_name_, 0, strlen(find_name) + 1);
		strcpy_s(find_name_, strlen(find_name) + 1, find_name);
	}
	static void time_thread()
	{
		for (int i = 0; i < 5; ++i)
		{
			boost::this_thread::sleep(boost::posix_time::seconds(1));
		}
	}
	template<typename T>
	void save(const char *file_path);

private:
	char * shared_name_;
	char * find_name_;
	size_t size_;
	long offset_;

	boost::interprocess::managed_shared_memory managed_shd;
	template<typename T>
	T * find() const
	{
		return (T *)managed_shd.get_address();
	};
};

#endif

template<typename T>
inline void server::save(const char * file_path)
{
	cout << "save begin..." << endl;
	FILE * stream_wr = NULL;
	fstream file;
	file.open(file_path, ios::out);//创建文件
	file.close();
	fopen_s(&stream_wr, file_path, "wb");
	boost::interprocess::named_mutex::remove("mtx");
	boost::interprocess::named_mutex::remove("mtx2");
	boost::interprocess::named_mutex named_mtx(boost::interprocess::open_or_create, "mtx");
	boost::interprocess::named_mutex named_mtx2(boost::interprocess::open_or_create, "mtx2");
	//boost::interprocess::named_condition named_cnd(boost::interprocess::open_or_create, "cnd");
	//boost::interprocess::scoped_lock<boost::interprocess::named_mutex> lock(named_mtx);
	char * p = (char *)this->find<T>();
	named_mtx.lock();
	while (1)
	{
		
		named_mtx.lock();
		long tmp_offset = fwrite(p, sizeof(char), strlen(p), stream_wr);
		offset_ += tmp_offset;
		fseek(stream_wr, offset_, SEEK_SET);
		memset(p, 0, sizeof(T) * strlen(p));
		named_mtx2.unlock();
		if (tmp_offset == 0)
			break;

	}
	fclose(stream_wr);
	cout << "save end..." << endl;
	
}
