#ifndef __NET_INTERFACE_H__
#define __NET_INTERFACE_H__
#include "INetInterface.h"
#include "uv.h"
class NetInterface : public INetInterface
{
public:
	virtual bool		StartListen(const IpPort& listenIpPort, IClientMgr* pClientMgr);
	//if StartListen called, then make sure pClientMgr is the same obj
	virtual IClient*	ConnectTo(const IpPort& desIpPort, IClientMgr* pClientMgr);

	virtual void		StopAll() { };

	virtual bool		SetLoggerLevel(int logLevel) { return true; };
	
	void OnDisconnect(IClient* client) { if (nullptr != m_pClientMgr) { m_pClientMgr->OnDisconnect(client); } }
	virtual				~NetInterface();
	NetInterface();
private:
	uv_tcp_t * server;
	IClientMgr* m_pClientMgr;
	static void on_connection(uv_stream_t* server, int status);
	void on_connection();
};

#endif

