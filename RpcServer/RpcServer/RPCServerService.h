#pragma once
#include "echo.pb.h"
#include "TcpConnect.h"
using namespace google::protobuf;

class RPCServerService: public echo::EchoService
{
public:
	RPCServerService(TcpConnection * con);
	~RPCServerService(void);
protected:
	TcpConnection * m_tcpCon;
};

//用于服务端
class EchoImplService :public RPCServerService{
public:
	EchoImplService(TcpConnection * con);
	void Echo(RpcController* controller,
		const echo::EchoRequest* request,
		echo::EchoResponse* response,
		Closure* done) ;
};

class EchoBackImplService :public RPCServerService{
private:
	echo::EchoService::Stub *m_stubService;
public:
	EchoBackImplService(TcpConnection * con);
	void Echo(RpcController* controller,
		const echo::EchoRequest* request,
		echo::EchoResponse* response,
		Closure* done) ;
};
