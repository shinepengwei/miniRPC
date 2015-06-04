#pragma once
#include "echo.pb.h"
#include "TcpConnect.h"
#include "rpcProxy.h"
using namespace google::protobuf;

class RPCServerService: public echo::EchoService
{
public:
	RPCServerService(TcpEntity * con, RpcProxy * pxy);
	~RPCServerService(void);
protected:
	TcpEntity * m_tcpCon;
	RpcProxy * m_proxy;
};


class EchoImplService :public RPCServerService{
public:
	EchoImplService(TcpEntity * con,RpcProxy * pxy);
	void Echo(RpcController* controller,
		const echo::EchoRequest* request,
		echo::EchoResponse* response,
		Closure* done) ;
};

class EchoBackImplService :public RPCServerService{
public:
	EchoBackImplService(TcpEntity * con,RpcProxy * pxy);
	void Echo(RpcController* controller,
		const echo::EchoRequest* request,
		echo::EchoResponse* response,
		Closure* done) ;
};
