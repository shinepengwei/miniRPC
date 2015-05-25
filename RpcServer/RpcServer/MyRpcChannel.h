#pragma once
#include <google\protobuf\service.h>
#include "TcpConnect.h"


using namespace google::protobuf;
class MyRpcChannel :
	public RpcChannel
{
public:
	MyRpcChannel(TcpConnection * tp);
	~MyRpcChannel(void);
	void CallMethod(const MethodDescriptor* method,
                          RpcController* controller,
                          const Message* request,
                          Message* response,
                          Closure* done);

private:
	TcpConnection * m_tcpClient;
};


