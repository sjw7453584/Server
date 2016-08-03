#ifndef __SQL_INTERFACE_H__
#define __SQL_INTERFACE_H__
#include "Singleton.h"
#include "DataBase.h"
#include <list>
#include <vector>
#include <unordered_map>
#include <thread>
#include <mutex>

class SqlTrans
{
public:
	virtual			~SqlTrans() {};
	virtual void 	OnExecute(DataBase* pDbConnect) = 0;
	virtual void 	OnFinish() = 0;
};

class SqlInterface
{
	DECLEAR_SINGLETON(SqlInterface)
public:
	SqlInterface();
	~SqlInterface();
	void AddAsyncTrans(uint32 index, SqlTrans* trans);
	SqlTrans* PopFinishedTrans();
	bool Init(std::vector<connection_info>& conn_infos, uint32 shared_threads_num = 0);
	void Run();
	void JoinDbThreads();
private:
	std::unordered_map<uint32, DataBase*> database_conns;
	std::unordered_map<uint32,std::list<SqlTrans*>> to_do_trans;
	std::list<SqlTrans*> finished_trans;
	static void db_thread(SqlInterface* sql_interface, uint32 conn_index);
	std::unordered_map<std::thread::id, std::thread*> db_threads;
	std::mutex to_do_mutex;
	std::mutex finished_mutex;
};
#endif // __SQL_INTERFACE_H__
