#include "SqlInterface.h"
#include <iostream>
IMPLEMENT_SINGLETON(SqlInterface)

SqlInterface::SqlInterface()
{
}
SqlInterface::~SqlInterface()
{
	for (auto conn : database_conns)
	{
		delete conn.second;
	}
	for (auto db : db_threads)
	{
		delete db.second;
	}
}

void SqlInterface::AddAsyncTrans(uint32 index, SqlTrans* trans)
{
	to_do_mutex.lock();
	to_do_trans[index].push_back(trans);
	to_do_mutex.unlock();
}

SqlTrans* SqlInterface::PopFinishedTrans()
{
	SqlTrans* trans = nullptr;

	finished_mutex.lock();
	if (!finished_trans.empty())
	{
		trans = finished_trans.front();
		finished_trans.pop_front();
	}
	finished_mutex.unlock();

	return trans;
}

bool SqlInterface::Init(std::vector<connection_info>& conn_infos)
{
	for (auto conn : conn_infos)
	{
		auto data_base = new DataBase();
		data_base->Init(conn);
		auto ptr = database_conns.find(conn.index);
		if (ptr != database_conns.end())
		{
			delete ptr->second;
		}
		database_conns[conn.index] = data_base;
		for (auto i = 0; i < conn.thread_num; i++)
		{
			auto db = new std::thread(db_thread, this, conn.index);
			db_threads[db->get_id()] = db;
		}

	}
	
	return true;
}

void SqlInterface::db_thread(SqlInterface* sql_interface, uint32 conn_index)
{ 
	for (;;)
	{
		auto& transes = sql_interface->to_do_trans[conn_index];
		SqlTrans* trans = nullptr;

		sql_interface->to_do_mutex.lock();
		if (!transes.empty())
		{
			trans = transes.front();
			transes.pop_front();
		}
		else
		{
			Sleep(10);
		}
		sql_interface->to_do_mutex.unlock();
		if (nullptr != trans)
		{
			trans->OnExecute(sql_interface->database_conns[conn_index]);
			std::cout << std::this_thread::get_id();
			sql_interface->finished_mutex.lock();
			sql_interface->finished_trans.push_back(trans);
			sql_interface->finished_mutex.unlock();
		}
	}
}

void SqlInterface::Run()
{
	for (;;)
	{
		SqlTrans* sqlTrans = PopFinishedTrans();
		if (nullptr != sqlTrans)
		{
			sqlTrans->OnFinish();
			delete sqlTrans;
		}
		else
		{
			Sleep(1);
		}
	}
}

void SqlInterface::JoinDbThreads()
{
	for (auto ptr : db_threads)
	{
		if (nullptr != ptr.second)
		{
			ptr.second->join();
		}
	}
}