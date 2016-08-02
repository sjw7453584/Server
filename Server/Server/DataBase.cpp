#include "DataBase.h"
#include "mysql.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Config.h"
#include <fstream>
#include "textReader.h"
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



bool DataBase::Init()
{
	init_mutex.lock();
	auto mysql_handle = mysql_init(nullptr);
	init_mutex.unlock();
	if (nullptr == mysql_real_connect(mysql_handle, "192.168.1.72", "root", "Sanguo1!", "zgame6", 3306, nullptr, 0))
	{
		std::cout << "failed to connect to mysql server\n";
		std::cout << mysql_error(mysql_handle) << std::endl;
	}

	std::ostringstream oss;
	oss << "create database if not exists `my_data_base` default character set `utf8`";
	if (0 != mysql_query(mysql_handle, oss.str().c_str()))
	{
		std::cout<<mysql_error(mysql_handle)<<std::endl;
	}
	oss.clear();
	oss.str("");

	oss << "use `my_data_base`";
	if (0 != mysql_query(mysql_handle, oss.str().c_str()))
	{
		std::cout << mysql_error(mysql_handle) << std::endl;
	}
	oss.clear();
	oss.str("");

	std::vector<std::string> sqlsList;
	oss << PROJECT_SOURCE_DIR << "/zgame2.sql";
	//oss << PROJECT_SOURCE_DIR << "/test.txt";
	auto rret = mysql_set_character_set(mysql_handle, "gb18030");
	if (0 != rret)
	{
		std::cout << mysql_error(mysql_handle) << std::endl;
	}
	auto  ret = ParseSqlFileDb(oss.str().c_str(), sqlsList);
	for (auto sql : sqlsList)
	{
		std::string str = /*s2utfs*/(sql);
		if (0 != mysql_real_query(mysql_handle, str.c_str(), str.length()))
		{
			std::cout << mysql_error(mysql_handle) << std::endl;
		}
	}
	return true;
}

