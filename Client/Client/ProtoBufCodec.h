
// Author: Albertli

#ifndef PROTOBUF_CODEC_H
#define PROTOBUF_CODEC_H

#include "google/protobuf/message.h"
#include <string>
#include "IGame.pb.h"
#include "IGameHead.pb.h"
#include "Types.h"

class ProtoBufCodec
{
public:
static bool						Encode(const google::protobuf::Message& inMessage, 
											std::string& outBuf, int32 errorNo = IGameProto::SUCCEED);

static google::protobuf::Message*	Decode(const char* pBuf, int len,
											IGameProto::MsgHead& msgHead);


};

google::protobuf::Message* createMessage(const std::string& type_name);

int32 asInt32(const char* buf);
int32 CheckSum(const char* buff, int startIndex, int lenth);


void SelfGameLogHandler(google::protobuf::LogLevel level, const char* filename, int line,
	const std::string& message);

#endif  // PROTOBUF_CODEC_H