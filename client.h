#ifndef CLIENT_H_
#define CLIENT_H_
#include <boost\interprocess\managed_shared_memory.hpp>
#include <iostream>
#include <boost\thread.hpp>
#include <boost\interprocess\sync\named_condition.hpp>
#include <boost\interprocess\sync\named_mutex.hpp>
#include <fstream>
using namespace std;
class client
{
public:
	client(const char * managed_name, size_t size)
		:managed_shd_(boost::interprocess::open_or_create, managed_name, size),
		size_(size),
		address_(NULL)
	{
		managed_name_ = new char[strlen(managed_name) + 1];
		mem_buffer_ = new char[size + 1];
		memset(mem_buffer_, 0, size + 1);
		memset(managed_name_, 0, strlen(managed_name) + 1);
		strcpy_s(managed_name_, strlen(managed_name) + 1, managed_name);
	}


	template<typename T>
	void init_memory();

	void writeToMemory(const char * file_path);
	

	template<typename T>
	T * find() const
	{
		return (T *)managed_shd_.get_address();
	};
private:
	char * managed_name_;
	char * address_;
	size_t size_;
	char * mem_buffer_;
	boost::interprocess::managed_shared_memory managed_shd_;


};
template<typename T>
inline void client::init_memory()
{
	address_ = this->find<T>();
	memset(address_, 0, size_);
}
#endif
