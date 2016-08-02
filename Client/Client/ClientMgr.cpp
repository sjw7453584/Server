#include "ClientMgr.h"
#include "Types.h"
#include "Client.h"
IMPLEMENT_SINGLETON(ClientMgr)

void ClientMgr::OnConnect(IClient* pIClient)
{

}

void ClientMgr::OnDisconnect(IClient* pIClient)
{
	ReleaseClient(pIClient);
}


Client*	ClientMgr::FindClient(uint32 uid)
{
	auto client = Find(uid);
	if (nullptr != client)
	{
		return (Client*)client;
	}
	return nullptr;
}

IClient*	ClientMgr::CreateClient()
{
	uint32 tempuid = CreateUid();
	if (tempuid == 0)
	{
		return nullptr;
	}
	Client* pClient = new Client();
	pClient->uid = tempuid;

	AddClient(pClient->uid, pClient);
	return pClient;
}
void		ClientMgr::ReleaseClient(IClient* pClient)
{
	if (nullptr != pClient)
	{
		EraseClient(((Client*)pClient)->uid);
		ReleaseUid(((Client*)pClient)->uid);
		delete pClient;
	}
}

void ClientMgr::BroadToALlClient(const google::protobuf::Message& msg, const Client* pExcludeClient)
{
		auto itr = mapClients.begin();
	for (; itr != mapClients.end(); ++itr)
	{
		auto pClient = itr->second;
		if (pClient != NULL && pClient != pExcludeClient)
		{
			((Client*)pClient)->Send(msg);
		}
	}
}

bool ClientMgr::AddClient(uint32 uid, IClient* Client)
{
	auto itr = mapClients.find(uid);
	if (itr != mapClients.end())
	{
		return false;
	}
	mapClients.insert(std::make_pair(uid, Client));
	return true;
}

IClient* ClientMgr::Find(uint32 uid)
{
	auto itr = mapClients.find(uid);
	if (itr != mapClients.end())
	{
		return itr->second;
	}
	return nullptr;
}

bool ClientMgr::EraseClient(uint32 uid)
{
	auto itr = mapClients.find(uid);
	if (itr != mapClients.end())
	{
		mapClients.erase(itr);
		return true;
	}
	return false;
}

void ClientMgr::CloseAll()
{
	auto itr = mapClients.begin();
	while (itr != mapClients.end())
	{
		auto pClient = itr->second;
		++itr;
		//pClient->Close();
	}
}

int	ClientMgr::GetMaxConnectsNum()
{
	return 1000;
}
ClientMgr::ClientMgr()
{
	uint32 maxConnects = GetMaxConnectsNum();
	for (uint32 i = 1; i < maxConnects + 1; ++i)
	{
		uint32 temp = i;
		m_uidCreator.push_back(temp);
	}
}

uint32	ClientMgr::CreateUid()
{
	if (m_uidCreator.empty())
	{
		//ErrorLog("ClientMgr uids all used");
		return 0;
	}
	uint32 uid = m_uidCreator.front();
	m_uidCreator.pop_front();
	return uid;
}

void ClientMgr::ReleaseUid(uint32 uid)
{
	m_uidCreator.push_back(uid);
}