#pragma once
#include "INetInterface.h"
#include "Types.h"
#include "Singleton.h"
#include <list>
#include <unordered_map>
#include "Client.h"
#include "google/protobuf/message.h"
class ClientMgr :public IClientMgr
{
	DECLEAR_SINGLETON(ClientMgr)
	virtual int			GetMaxConnectsNum();
	virtual IClient*	CreateClient();
	virtual void		ReleaseClient(IClient*);

	virtual void		OnConnect(IClient*);
	virtual void		OnDisconnect(IClient*);

	virtual void		ReqConnectCallBack(int errorNo, void* pArg, IClient* pClient) {}
	void BroadToALlClient(const google::protobuf::Message& msg, const Client* pExcludeClient);
	Client*	FindClient(uint32 uid);
	ClientMgr();
private:
	uint32	CreateUid();
	void CloseAll();
	bool EraseClient(uint32 uid);
	IClient* Find(uint32 uid);
	bool AddClient(uint32 uid, IClient* Client);
	void ReleaseUid(uint32 uid);
	std::unordered_map<uint32, IClient*> mapClients;
	std::list<uint32> m_uidCreator;
};