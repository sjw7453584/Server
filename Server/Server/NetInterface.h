#ifndef __NET_INTERFACE_H__
#define __NET_INTERFACE_H__

#include "INetInterface.h"
class uv_stream_t;
class NetInterface : public INetInterface
{
public:
	virtual bool		StartListen(const IpPort& listenIpPort, IClientMgr* pClientMgr);

	virtual IConnector*	StartConnector(IClientMgr* pClientMgr) {};

	//return 0: timeout -1: epoll_wait err      -2: stopServer
	virtual int			Run(int timeout) {};

	virtual bool		WakeUp() {};

	virtual void		StopAll() {};

	virtual bool		SetLoggerLevel(int logLevel) {};
	static void on_connection(uv_stream_t* server, int status);
	virtual				~NetInterface() {};
};

#endif

