#include "echo.pb.h"
#include "MyRpcChannel.h"
#include "TcpServer.h"
using namespace google::protobuf;
class RpcProxy{
private:
	RpcChannel * channel;
	echo::EchoService * service;
public:
	RpcProxy(TcpServer * server){
		channel = new MyRpcChannel(server);
		
	}
	void Echo(std::string string){
		echo::EchoRequest request;
		request.set_message(string);
		service = new echo::EchoService::Stub(channel);
		service->Echo(NULL, &request, NULL, NULL);
	}
};