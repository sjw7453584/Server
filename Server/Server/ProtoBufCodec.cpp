#include "ProtoBufCodec.h"
#ifdef WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>  // htonl, ntohl
#endif
//#include "ServerLog.h"
#include "google/protobuf/descriptor.h"
#include "Types.h"
#include <stdint.h>
#include <stdio.h>
#define ErrorLog printf

using namespace IGameProto;
int32 asInt32(const char* buf)
{
	int32 be32 = 0;
	::memcpy(&be32, buf, sizeof(be32));
	return ::ntohl(be32);
}
int16 asInt16(const char* buf)
{
	int16 be16 = 0;
	::memcpy(&be16, buf, sizeof(be16));
	return ::ntohs(be16);
}

int CheckSum(const char* buff, int startIndex, int lenth)
{
	int checkSum = 0;
	for (int i = 0; i < (lenth-4) / 4; ++i)
	{
		checkSum += asInt32(buff+startIndex+4+i * 4);
	}
	return checkSum;
}

bool	ProtoBufCodec::Encode(const google::protobuf::Message& inMessage,
									std::string& outBuf, int32 errorNo)
{
	MsgHead msgHead;
	std::string typeName = inMessage.GetDescriptor()->name();
	msgHead.set_errorcode(errorNo);
	msgHead.set_msgtypename(typeName);
	
	outBuf.resize(sizeof(int32)+sizeof(int16));

	msgHead.AppendToString(&outBuf);

	int16 headLen = outBuf.size()-4;
	headLen = htons(headLen);
	memcpy( (void*)(outBuf.c_str()+4), &headLen , sizeof(int16));
	//if (errorNo == 0)
	{
		inMessage.AppendToString(&outBuf);
	}
	int32 totalLen = outBuf.size()+4;
	totalLen = htonl(totalLen);
	memcpy( (void*)(outBuf.c_str()), &totalLen , sizeof(int32));

	int32 checkSum = htonl(CheckSum(outBuf.c_str(), 0, outBuf.size()));
	outBuf.append((char*)&checkSum,sizeof(int32));
	
	return true;
}


google::protobuf::Message* createMessage(const std::string& type_name)
{
  google::protobuf::Message* message = NULL;
  const google::protobuf::Descriptor* descriptor =
    google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(type_name);
  if (descriptor)
  {
    const google::protobuf::Message* prototype =
      google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
    if (prototype)
    {
      message = prototype->New();
    }
  }
  return message;
}


google::protobuf::Message* ProtoBufCodec::Decode(const char* pBuf, int len, MsgHead& msgHead)
{
	msgHead.Clear();
	google::protobuf::Message* result = NULL;

	if (len >= 10)
	{
		int32 totalLen = asInt32(pBuf);
		if( totalLen != len)
		{
			ErrorLog("inbuf totalLen=%d != buflen=%d", totalLen, len);
			return NULL;
		}
		int16 headLen = asInt16(pBuf+4);
		if( headLen <= 0 || headLen > totalLen-8)
		{
			ErrorLog("inbuf headLen=%d totallen=%d", headLen, totalLen);
			return NULL;
		}
		int32 expectedCheckSum = asInt32(pBuf + len - 4);
		
		int32 checkSum = CheckSum(pBuf, 0, len-4);
	
		if (checkSum == expectedCheckSum)
		{
			bool ret = msgHead.ParseFromArray(pBuf+6, headLen-2);
			if( !ret)
			{
				ErrorLog("msgHead Parse error");
				return NULL;
			}
			std::string typeName = "IGameProto.";
			typeName += msgHead.msgtypename();
			google::protobuf::Message* message = createMessage(typeName);
			if (message)
			{
				const char* data = pBuf + 4 + headLen;
				int32 dataLen = len - headLen - 8;
				if (message->ParseFromArray(data, dataLen))
				{
					result = message;
				}
				else
				{
					ErrorLog("ParseFromArray typeName=%s error", typeName.c_str());
					delete message;
					return NULL;
				}
			}
			else
			{
				ErrorLog("createMessage NULL typeName=%s", typeName.c_str());
				return NULL;
			}
			
		}
		else
		{
			ErrorLog("check sum error: expectedCheckSum=%d checkSum=%d",
				expectedCheckSum,checkSum);
		}
	}
	else
	{
		ErrorLog("decode buf len=%d", len);
	}

	return result;
}
void SelfGameLogHandler(google::protobuf::LogLevel level, const char* filename, int line,
	const std::string& message)
{
	static char* temp = NULL;
	if (temp == NULL)
	{
		temp = new char[1000];
	}
	// We use fprintf() instead of cerr because we want this to work at static
	// initialization time.
	sprintf(temp, "proto [%s:%d] %s",
		filename, line, message.c_str());
	if( level == google::protobuf::LOGLEVEL_ERROR)
	{
		ErrorLog(temp);
	}
	//else if( level == google::protobuf::LOGLEVEL_INFO)
	//{
	//	InfoLog(temp);
	//}
	//else if( level == google::protobuf::LOGLEVEL_FATAL)
	//{
	//	FatalLog(temp);
	//}
	//else if( level == google::protobuf::LOGLEVEL_WARNING)
	//{
	//	WarnLog(temp);
	//}
	//else
	//{
	//	ShowLog(temp);
	//}
}