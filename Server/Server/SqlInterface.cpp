#include "SqlInterface.h"
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
	}

	auto db = new std::thread(db_thread, this);
	db_threads[db->get_id()] = db;
	
	return true;
}

void SqlInterface::db_thread(SqlInterface* sql_interface)
{
	for (;;)
	{
		for (auto& ptr : sql_interface->to_do_trans)
		{
			if (!ptr.second.empty())
			{
				SqlTrans* trans = nullptr;
				sql_interface->to_do_mutex.lock();
				if (!ptr.second.empty())
				{
					trans = ptr.second.front();
					ptr.second.pop_front();
				}
				sql_interface->to_do_mutex.unlock();
				if (nullptr != trans)
				{
					trans->OnExecute(sql_interface->database_conns[ptr.first]);
				}
				sql_interface->finished_mutex.lock();
				sql_interface->finished_trans.push_back(trans);
				sql_interface->finished_mutex.unlock();
				break;
			}

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