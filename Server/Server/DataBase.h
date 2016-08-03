#ifndef __DATA_BASE_H__
#define __DATA_BASE_H__
#include <mutex>
#include "Types.h"
#include "mysql.h"
struct connection_info
{
	uint32 index;
	std::string ip;
	uint32 port;
	std::string user_name;
	std::string passwd;
	std::string database_name;
	std::string conn_codec;
	std::string mysql_file;
	uint32 thread_num;
};

class QueryResult;

class DataBase
{
public:
	DataBase();
	~DataBase();
	bool Init(connection_info & conn_info);
	QueryResult Query(std::string query_str, int32& erroNo);

private:
	std::mutex init_mutex;
	std::mutex query_mutex;
	MYSQL* mysql_handle;
};
#endif
