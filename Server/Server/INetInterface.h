#ifndef __INET_INTERFACE_H__
#define __INET_INTERFACE_H__

#define IP_STR_MAX_LENTH 16

struct IpPort
{
	char			szIp[IP_STR_MAX_LENTH];
	unsigned short	port;
};

class IConnect
{
public:
	virtual bool Send(void* pBuf, int len) = 0;
	virtual void Close() = 0;
	virtual void GetClientIpPort(unsigned int& ip, unsigned short& port) = 0;

	virtual ~IConnect() {};
};

class IClient
{
public:
	virtual void OnConnect() = 0;

	virtual int	 ParsePkgLen(void* pBuf, int len) = 0;
	virtual void OnRecv(void* pBuf, int len) = 0;

	virtual void OnDisconnect() = 0;

	virtual ~IClient() {};
};

//dll使用者继承并实现
class IClientMgr
{
public:
	virtual int			GetMaxConnectsNum() = 0;
	virtual IClient*	CreateClient(IConnect* pConnect) = 0;
	virtual void		ReleaseClient(IClient*) = 0;

	virtual void		OnConnect(IClient*) = 0;
	virtual void		OnDisconnect(IClient*) = 0;

	virtual void		ReqConnectCallBack(int errorNo, void* pArg, IClient* pClient) = 0;

	virtual ~IClientMgr() {};
};

//*********************************************************
//**********************dll内部实现 ***********************
//*********************************************************

class IConnector
{
public:
	virtual bool		ReqConnect(const IpPort& hostIpPort, void* pArg) = 0;

	virtual ~IConnector() {};
};

class INetInterface
{
public:
	virtual bool		StartListen(const IpPort& listenIpPort, IClientMgr* pClientMgr) = 0;

	virtual IConnector*	StartConnector(IClientMgr* pClientMgr) = 0;

	//return 0: timeout -1: epoll_wait err      -2: stopServer
	virtual int			Run(int timeout) = 0;

	virtual bool		WakeUp() = 0;

	virtual void		StopAll() = 0;

	virtual bool		SetLoggerLevel(int logLevel) = 0;

	virtual				~INetInterface() {};

	static INetInterface* m_pInstance;
	static INetInterface *GetInstance();
};

#ifdef WIN32
#define DLLEXPORT __declspec(dllexport)  
#else
#define DLLEXPORT
#endif

#ifdef	__cplusplus
extern   "C" {
#endif
	DLLEXPORT INetInterface*		GetNetInterface();
	typedef INetInterface*		(*FuncAddr_GetNetInterface)();

#ifdef	__cplusplus
}
#endif 


#endif
