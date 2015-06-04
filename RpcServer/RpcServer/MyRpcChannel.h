#pragma once
#include <google\protobuf\service.h>
#include "TcpConnect.h"


using namespace google::protobuf;
class MyRpcChannel :
	public RpcChannel
{
public:
	MyRpcChannel(TcpEntity * tp);
	~MyRpcChannel(void);
	void CallMethod(const MethodDescriptor* method,
                          RpcController* controller,
                          const Message* request,
                          Message* response,
                          Closure* done);

private:
	TcpEntity * m_tcpClient;
};


