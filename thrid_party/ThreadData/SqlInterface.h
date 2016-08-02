/*
 * AsynInterface.h
 *
 *  Created on: 2011-7-20
 *      Author: albertli
 */

#ifndef ASYN_INTERFACE_H_
#define ASYN_INTERFACE_H_

#include "SimpleThread.h"
#include <string>
#include "CircleQueue.h"
#include "Singleton.h"
#include "Database/Database.h"
#include "hiredis.h"

using namespace std;

class SqlTrans
{
public:
	virtual			~SqlTrans(){};
	virtual void 	OnExecute(Database* pDbConnect) = 0;
	virtual void 	OnFinish() = 0;
};

struct DbConnectInfo
{
	int				hashStartIndex;
	const char*		pszInfoString;
	int				threadsNum;


	DbConnectInfo()
		:hashStartIndex(0)
		,pszInfoString(NULL)
		,threadsNum(0)
	{}
};

class SqlConnect;
class QueryResult;
class SqlInterface : public Singleton<SqlInterface>
{
public:
	SqlInterface();
	virtual ~SqlInterface();
	bool 			Init(const DbConnectInfo *connectsInfos, int connectsNum, int logLevel,
								bool needSyn = false);
	
	int 			AddAsynTrans(const SqlTrans* psqlTrans,unsigned int hashValue=0);
	

	SqlTrans*		PopFinishedTrans();


	
	unsigned long	SynEscapeString(char *to, const char *from, 
		unsigned long length,
		unsigned int hashValue = 0);

	QueryResult* 	SynQuery(const char* pszSql, unsigned length, 
		int& errerNo, unsigned int hashValue = 0);
	int				GetHashIndex(unsigned int hashValue);
	const DbConnectInfo* GetConnectsInfo(int& connectNum);
private:
	bool			InitMysql();
	
	SqlConnect*		m_pSqlConnects;
	int				m_connectsNum;
	DbConnectInfo*	m_connectsInfos;
	int				m_totalHashUidNum;
	bool			isInitMysql;

};

#endif /* ASYNINTERFACE_H_ */
