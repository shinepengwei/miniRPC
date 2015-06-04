#pragma once
#include "echo.pb.h"
#include "MyRpcChannel.h"
using namespace google::protobuf;
//User和ServerService调用RPC时，使用proxy
class RpcProxy{
private:
	RpcChannel * m_channel;
	echo::EchoService * service;
public:
	RpcProxy(TcpEntity * server);
	void Echo(std::string string);
};