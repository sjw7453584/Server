#include "Client.h"
#include "ProtoBufCodec.h"
#include <iostream>
void Client::OnConnect()
{
	std::cout << "onconnect in Client";
}

int	 Client::ParsePkgLen(void* pBuf, int len)
{
	return 0;
}

void Client::OnRecv(void* pBuf, int len)
{
	IGameProto::MsgHead msgHead;
	google::protobuf::Message* pReqMsg =
		ProtoBufCodec::Decode((const char*)pBuf, len, msgHead);
	if (pReqMsg)
	{
		/*++msgsCount;
		msgNamesList.push_back(msgHead.msgtypename());
		if (ClientMgr::GetSingleton().m_nowTime - lastRecvTime >= 1)
		{
			lastRecvTime = ClientMgr::GetSingleton().m_nowTime;
			if (msgsCount >= 90)
			{
				string buf;
				buf += "msg too pingfan ";
				buf += ": ";
				for (int i = 0; i < msgNamesList.size(); ++i)
				{
					buf += msgNamesList[i];
					buf += " ";
				}
				ErrorLog(buf.c_str());

				Close();
				return;
			}
			msgNamesList.clear();
			msgsCount = 0;
		}*/

		//ProcessClientMsg::GetSingleton().Process(this, msgHead, *pReqMsg);
		delete pReqMsg;
	}
	/*else
	{
		Close();
		ErrorLog("uid=%u decode err, close connect", uid);
	}*/

	Send("received", strlen("received")+ 1);
}

void Client::OnDisconnect()
{
	std::cout << "OnDisconnect in Client";

}

bool Client::Send(const google::protobuf::Message& msg, int32 errorCode)
{
	std::string outBuf;
	ProtoBufCodec::Encode(msg, outBuf, errorCode);
	//if (msg.GetTypeName().find("MsgHeartBeat") == std::string::npos)
	//{
	//	//InfoLog(msg.DebugString().c_str());
	//}

	return Client::Send(outBuf.c_str(), outBuf.size());
}

bool Client::Send(const google::protobuf::Message& msg, const std::string& errorCodeStr)
{
	int32 errorCode = 0;
	std::string outBuf;
	ProtoBufCodec::Encode(msg, outBuf, errorCode);
	/*if (msg.GetTypeName().find("MsgHeartBeat") == std::string::npos)
	{
		InfoLog(msg.DebugString().c_str());
	}*/
	return Client::Send(outBuf.c_str(), outBuf.size());
}

bool Client::Send(const char* pBuf, int len)
{
	return m_pConn->Send((void*)pBuf, len);
}
