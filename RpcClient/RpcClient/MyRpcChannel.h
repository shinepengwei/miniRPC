#pragma once
#include <google\protobuf\service.h>
namespace google {
namespace protobuf {

class MyRpcChannel :
	public RpcChannel
{
public:
	MyRpcChannel(void);
	~MyRpcChannel(void);
	void CallMethod(const MethodDescriptor* method,
                          RpcController* controller,
                          const Message* request,
                          Message* response,
                          Closure* done);

private:

};


}}
