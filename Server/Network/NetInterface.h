#ifndef __NET_INTERFACE_H__
#define __NET_INTERFACE_H__
#include "INetInterface.h"
class NetInterface : public INetInterface
{
public:
	virtual bool		StartListen(const IpPort& listenIpPort, IClientMgr* pClientMgr);

	virtual IConnector*	StartConnector(IClientMgr* pClientMgr) { return nullptr; };

	//return 0: timeout -1: epoll_wait err      -2: stopServer
	virtual int			Run(int timeout) { return 0; };

	virtual bool		WakeUp() { return true; };

	virtual void		StopAll() { };

	virtual bool		SetLoggerLevel(int logLevel) { return true; };
	static void on_connection(uv_stream_t* server, int status);
	void on_connection();
	void OnDisconnect(IClient* client) { if (nullptr != m_pClientMgr) { m_pClientMgr->OnDisconnect(client); } }
	virtual				~NetInterface();
	NetInterface();
private:
	uv_tcp_t * server;
	IClientMgr* m_pClientMgr;
};

#endif

