#include "NetInterface.h"
#include <iostream>
#include <functional>
#include "uv.h"
#include "NetClient.h"
#include "NetConnector.h"

INetInterface* INetInterface::m_pInstance = nullptr;

INetInterface *INetInterface::GetInstance()
{
	if (nullptr == m_pInstance)
	{
		m_pInstance = new NetInterface();
	}

	return m_pInstance;
}

NetInterface::NetInterface():server(nullptr), m_pClientMgr(nullptr)
{

}

NetInterface::~NetInterface()
{
	if (nullptr != server)
	{
		delete server;
		server = nullptr;
	}
}

void NetInterface::on_connection()
{
	if (nullptr == m_pClientMgr)
	{
		std::cout << "no client manager";
		return;
	}
	auto client = m_pClientMgr->CreateClient();
	if (nullptr != client)
	{
		auto conn = new NetConnector();
		client->SetConnector(conn);
		conn->SetClient(client);
		conn->OnConnect((uv_stream_t*)server);
	}
}

void NetInterface::on_connection(uv_stream_t* server, int status)
{
	std::cout << "client come in";
	if (0 > status)
	{
		std::cout << uv_strerror(status) << "\n";
		return;
	}
	NetInterface* pNetInterface = static_cast<NetInterface*>(server->data);
	if (NULL != pNetInterface)
	{
		pNetInterface->on_connection();
	}
	

}

bool NetInterface::StartListen(const IpPort& listenIpPort, IClientMgr* pClientMgr)
{
	m_pClientMgr = pClientMgr;
	server = new uv_tcp_t();
	uv_tcp_init(uv_default_loop(), server);

	sockaddr_in addr;
	uv_ip4_addr(listenIpPort.szIp, listenIpPort.port, &addr);

	uv_tcp_bind(server, (sockaddr*)&addr, 0);
	server->data = this;
	uv_listen((uv_stream_t*)server, 100, NetInterface::on_connection);
	return true;
}
