#include "SelectRole.h"
#include <string.h>
#include "ProtoBufCodec.h"
#include <sstream>
#include "QueryResult.h"
#include <iostream>

SelectRole::SelectRole(uint32 uId, uint32 roleId)
	:_uid(uId), _role_id(roleId)
{
}

//dbthread
void 	SelectRole::OnExecute(DataBase* pDbConnect)
{
	std::ostringstream ss;
	ss << "select "
		<< "`accoutId`, `roleId`, `roleName`, `creatureType`, `sceneId`, `X`, `Y`,"
		<< "UNIX_TIMESTAMP(createTimeStamp), UNIX_TIMESTAMP(lastTiliUpdateTimeStamp), `curFormId`,"
		<< "`gold`, `silver`, `copper`, `countryId`, `armyId`, `armyMemberType`, `armyOfficerType`, `offices`, `cityId`"
		" from `User` where `roleId`='" << _role_id << "'";

	QueryResult pResult = pDbConnect->Query(ss.str(), errNo);
	for (auto ptr : pResult.AllRows())
	{
		if (!ptr.second.empty())
		{

		}
	}
}

//在游戏主线程执行;
void 	SelectRole::OnFinish()
{
	if (0 != errNo)
	{
		std::cout << "exe mysql failed";
	}
	std::cout << "finished";
}

void getSelectRole(uint32 uId, uint32 roleId)
{
	SqlTrans* pTrans = new SelectRole(uId, roleId);
	//SqlTrans* pTrans1 = new SelectRole(uId, roleId);
	SqlInterface::GetSingleton().AddAsyncTrans(1, pTrans);
	//SqlInterface::GetSingleton().AddAsyncTrans(2, pTrans1);
}