#include "rpcProxy.h"

RpcProxy::RpcProxy(TcpConnection * server){
	m_channel = new MyRpcChannel(server);
		
}
void RpcProxy::Echo(std::string string){
	echo::EchoRequest request;
	request.set_message(string);
	service = new echo::EchoService::Stub(m_channel);
	service->Echo(NULL, &request, NULL, NULL);
}