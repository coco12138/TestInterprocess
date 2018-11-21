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

	void writeToMemory(const char * file_path);	//write file contents to shared-memory
	

	template<typename T>
	T * find() const				//get shared-memory address
	{
		return (T *)managed_shd_.get_address();
	};
private:
	char * managed_name_;		//shared-memory name
	char * address_;		//shared-memory address
	size_t size_;			//shared-memory size
	char * mem_buffer_;		//buffer used to save temperaty data
	boost::interprocess::managed_shared_memory managed_shd_;	//boost managed_shared_memory


};
template<typename T>
inline void client::init_memory()	//initialize shared-memory, set all to zero
{
	address_ = this->find<T>();
	memset(address_, 0, size_);
}
#endif
