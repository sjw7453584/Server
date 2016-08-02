#ifndef __DATA_BASE_H__
#define __DATA_BASE_H__
#include <mutex>

class DataBase
{
public:
	DataBase();
	~DataBase();
	bool Init();
private:
	std::mutex init_mutex;
};
#endif
