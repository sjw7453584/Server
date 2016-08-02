#pragma once
#include "INetInterface.h"
#include "Types.h"
#include <string>
#include "google/protobuf/message.h"

class Client :public IClient
{
public:
	virtual void OnConnect();

	virtual int	 ParsePkgLen(void* pBuf, int len);
	virtual void OnRecv(void* pBuf, int len);

	virtual void OnDisconnect();
	bool Send(const google::protobuf::Message& msg, int32 errorCode = 0);
	bool Send(const google::protobuf::Message& msg, const std::string& errorCodeStr);
	bool Send(const char* pBuf, int len);
	uint32 uid;
};