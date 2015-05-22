#pragma once
#include <google\protobuf\service.h>
#include "TcpClient.h"
namespace google {
namespace protobuf {

class MyRpcChannel :
	public RpcChannel
{
public:
	MyRpcChannel(TcpClient * tp);
	~MyRpcChannel(void);
	void CallMethod(const MethodDescriptor* method,
                          RpcController* controller,
                          const Message* request,
                          Message* response,
                          Closure* done);

private:
	TcpClient * m_tcpClient;


};


}}
