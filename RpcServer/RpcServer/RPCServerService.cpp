#include "stdafx.h"
#include "RPCServerService.h"
#include <iostream>

RPCServerService::RPCServerService(TcpConnection * con):m_tcpCon(con){
	con->addService(this);
}
RPCServerService::~RPCServerService(void){}




EchoImplService::EchoImplService(TcpConnection * con):RPCServerService(con){}
void EchoImplService::Echo(RpcController* controller,
	const echo::EchoRequest* request,
	echo::EchoResponse* response,
	Closure* done) {
		std::cout<<"                                RPC,message:"<<request->message()<<std::endl;
	}

EchoBackImplService::EchoBackImplService(TcpConnection * con):RPCServerService(con){
	m_stubService = new echo::EchoService::Stub(con);
}
void EchoBackImplService::Echo(RpcController* controller,
	const echo::EchoRequest* request,
	echo::EchoResponse* response,
	Closure* done) {
		std::cout<<"                                RPC,message:"<<request->message()<<std::endl;
		//TODO ÄÜ·ñÌáÈ¡
		echo::EchoRequest requestBack;
		requestBack.set_message(request->message());
		m_stubService->Echo(NULL, &requestBack, NULL, NULL);
		//m_proxy->Echo(request->message());
	}