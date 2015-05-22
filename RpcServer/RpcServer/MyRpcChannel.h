#pragma once
#include <google\protobuf\service.h>
#include "TcpServer.h"
namespace google {
namespace protobuf {

class MyRpcChannel :
	public RpcChannel
{
public:
	MyRpcChannel(TcpServer * tp);
	~MyRpcChannel(void);
	void CallMethod(const MethodDescriptor* method,
                          RpcController* controller,
                          const Message* request,
                          Message* response,
                          Closure* done);

private:
	TcpServer * m_tcpClient;


};


}}
