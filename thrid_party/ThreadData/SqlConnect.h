/*
 * SqlConnect.h
 *
 *  Created on: 2012-2-20
 *      Author: albertli
 */

#ifndef MYSQL_CONNECT_H_
#define MYSQL_CONNECT_H_

#include "SimpleThread.h"
#include <string>
#include "CircleQueue.h"
#include "Database/Database.h"
#include "ConditionEvent.h"
#include "Lock.h"
using namespace std;

#define MaxThreadsNum 10
class QueryResult;
class SqlTrans;
class SqlConnect
{
public:
	SqlConnect();
	bool 			Init(const char *infoString, int threadNum,bool needSyn, int hashStartIndex);
	unsigned long	SynEscapeString(char *to, const char *from, unsigned long length);
	QueryResult* 	SynQuery(const char* pszSql, unsigned length, int& errerNo);

	int 			AddAsynTrans(const SqlTrans* psqlTrans);
	
	SqlTrans*		PopFinishedTrans();
	void			Stop();
	
#ifdef WIN32
	static unsigned int (__stdcall WorkThreadFun)(LPVOID param);
#else
	static void* 	WorkThreadFun(void* param);
#endif

	
	int				m_hashStartIndex;
	const char*		dbConnectInfo;
	SimpleThread*	m_arrpthreads[MaxThreadsNum];

    CircleQueue m_reqCircleQueue;
	CircleQueue m_resultCircleQueue;

	string		m_infoString;
	Database*	m_pSynDatabase;
	ConditionEvent m_event;
	MutexLock m_lock;
	volatile int threadSucceedCount;
	bool m_needSyn;
	bool m_stop;
};

#endif /* ASYNINTERFACE_H_ */
