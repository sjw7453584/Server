#include "DataBase.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Config.h"
#include <fstream>
#include "textReader.h"
#ifdef WIN32
//#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
//#include <sys/stat.h>
//#include <sys/types.h>
#endif
#include "QueryResult.h"

std::string TrimEndStr(const std::string& src, const char* charsTrim)
{
	int start = 0;
	int end = src.length();
	for (; start < end; ++start)
	{
		bool isFound = false;
		for (int k = 0; charsTrim[k] != 0; ++k)
		{
			if (src[start] == charsTrim[k])
			{
				isFound = true;
				break;
			}
		}

		if (!isFound)
			break;
	}

	for (--end; end >= start; --end)
	{
		bool isFound = false;
		for (int k = 0; charsTrim[k] != 0; ++k)
		{
			if (src[end] == charsTrim[k])
			{
				isFound = true;
				break;
			}
		}
		if (!isFound)
			break;
	}
	++end;
	if (end > start)
	{
		return src.substr(start, end);
	}
	return "";
}


std::vector<std::string> SpliteStr(std::string src, char seprator)
{
	std::vector<std::string> retList;
	auto pos = src.find(seprator);
	auto last_pos = -1;
	while ( std::string::npos != pos)
	{
		auto ele = src.substr(last_pos + 1, pos - last_pos - 1);
		if (!ele.empty())
		{
			retList.push_back(ele);
		}
		last_pos = pos;
		pos = src.find(seprator, last_pos + 1);
	}
	if (last_pos != -1)
	{
		auto last_ele = src.substr(last_pos + 1, src.length() - last_pos - 1);
		if (!last_ele.empty())
		{
			retList.push_back(last_ele);
		}
	}
	return retList;
}

bool ParseSqlFileDb(std::string sqlFilePath, std::vector<std::string>& sqlsList)
{
	std::ostringstream stream;
	auto reader = TextFactory::CreateText("utf-8", sqlFilePath);
	if (nullptr != reader)
	{
		reader->Read(stream);
		std::istringstream in_stream(stream.str());
		stream.clear();
		stream.str("");
		std::string line;
		while (std::getline(in_stream, line))
		{
			std::string templine = TrimEndStr(line, " \r\t\n");
			if (templine.find_first_of("--") == 0
				|| templine.find_first_of("/*") == 0
				|| templine.find_first_of("#") == 0
				|| templine.find_first_of("//") == 0
				|| templine.empty())
			{
				continue;
			}
			else
			{
				stream << templine;
			}
		}
	}
	
	sqlsList = SpliteStr(stream.str(), ';');
	return sqlsList.size() > 0;

}

DataBase::DataBase()
{

}

DataBase::~DataBase()
{

}

bool DataBase::Init(connection_info & conn_info)
{
	init_mutex.lock();
	mysql_handle = mysql_init(nullptr);
	init_mutex.unlock();
	if (nullptr == mysql_real_connect(mysql_handle, conn_info.ip.c_str(), conn_info.user_name.c_str(), conn_info.passwd.c_str(), "", conn_info.port, nullptr, 0))
	{
		std::cout << "failed to connect to mysql server\n";
		std::cout << mysql_error(mysql_handle) << std::endl;
	}

	std::ostringstream oss;
	oss << "create database if not exists `"<<conn_info.database_name<<"` default character set `" <<conn_info.conn_codec<<"`";
	if (0 != mysql_query(mysql_handle, oss.str().c_str()))
	{
		std::cout<<mysql_error(mysql_handle)<<std::endl;
	}
	oss.clear();
	oss.str("");

	oss << "use `"<<conn_info.database_name<<"`";
	if (0 != mysql_query(mysql_handle, oss.str().c_str()))
	{
		std::cout << mysql_error(mysql_handle) << std::endl;
	}
	oss.clear();
	oss.str("");

	
	
	//oss << PROJECT_SOURCE_DIR << "/test.txt";
	auto rret = mysql_set_character_set(mysql_handle, conn_info.conn_codec.c_str());
	if (0 != rret)
	{
		std::cout << mysql_error(mysql_handle) << std::endl;
	}

	std::vector<std::string> sqlsList;
	std::string sql_file_name{};
	if (!conn_info.mysql_file.empty())
	{
		sql_file_name += "./mysql/";
		sql_file_name += conn_info.mysql_file;
		int ret = access(sql_file_name.c_str(), 0);
		if (ret != 0)
		{
			oss << PROJECT_SOURCE_DIR << "/mysql/" << conn_info.mysql_file;
			sql_file_name = oss.str();
		}
	}

	if (!sql_file_name.empty())
	{
		auto  ret = ParseSqlFileDb(oss.str().c_str(), sqlsList);
		for (auto sql : sqlsList)
		{
			std::string str = /*s2utfs*/(sql);
			if (0 != mysql_real_query(mysql_handle, str.c_str(), str.length()))
			{
				std::cout << mysql_error(mysql_handle) << std::endl;
			}
		}
	}
	
	return true;
}

QueryResult DataBase::Query(std::string query_str, int32& erroNo)
{
	erroNo = 0;
	QueryResult result;
	if (0 != mysql_real_query(mysql_handle, query_str.c_str(), query_str.length()))
	{
		erroNo = mysql_errno(mysql_handle);
		std::cout << mysql_error(mysql_handle) << std::endl;
		return result;
	}

	auto res = mysql_store_result(mysql_handle);
	if (nullptr == res)
	{
		erroNo = mysql_errno(mysql_handle);
		if (0 != erroNo)
		{
			std::cout << mysql_error(mysql_handle) << std::endl;
		}
		return result;
		//auto cnt = mysql_field_count(mysql_handle);
	}
	auto field_number = mysql_field_count(mysql_handle);
	uint32 rowIndex = 0;
	while (auto row = mysql_fetch_row(res))
	{
		for (auto i = 0; i < field_number; i++)
		{
			//std::cout << row[i];
			Field f;
			f = row[i];
			result.AddField(rowIndex, f);
		}
		rowIndex++;
	}

	return result;
}

