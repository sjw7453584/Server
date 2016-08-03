#ifndef SelectRoleDbTrans_H
#define SelectRoleDbTrans_H
#include "SqlInterface.h"
#include "Types.h"
#include "IGame.pb.h"
using namespace std;
class Role;
class SelectRole : public SqlTrans
{
	uint32 _uid;
	uint32 _role_id;
	IGameProto::MsgSelectRoleAck ack;
	Role* role_info;
	int32 errNo;
public:
	SelectRole(uint32 uId, uint32 roleId);
	virtual void 	OnExecute(DataBase* pDbConnect);
	virtual void 	OnFinish();

};

void getSelectRole(uint32 uId, uint32 roleId);

#endif