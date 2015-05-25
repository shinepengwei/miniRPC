#include "stdafx.h"
#include "RPCServerService.h"
#include <iostream>

RPCServerService::RPCServerService(TcpConnection * con, RpcProxy * pxy):m_tcpCon(con),m_proxy(pxy){
	con->addService(this);
}
RPCServerService::~RPCServerService(void){}




EchoImplService::EchoImplService(TcpConnection * con,RpcProxy * pxy):RPCServerService(con,pxy){}
void EchoImplService::Echo(RpcController* controller,
	const echo::EchoRequest* request,
	echo::EchoResponse* response,
	Closure* done) {
		std::cout<<"RPC,message:"<<request->message()<<std::endl;
		m_proxy->Echo(request->message());
	}