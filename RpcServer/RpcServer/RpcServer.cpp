// RpcServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <stdio.h>   
#include "echo.pb.h"
#include "TcpServer.h"
#include <boost/thread/thread.hpp>   
#include "rpcProxy.h"
                 
#pragma comment(lib, "ws2_32.lib")
             
using namespace google::protobuf;
class EchoImplService : public echo::EchoService {
 public:
  void Echo(RpcController* controller,
              const echo::EchoRequest* request,
              echo::EchoResponse* response,
              Closure* done) {
				  std::cout<<"RPC,message:"<<request->message()<<std::endl;
				  proxy->Echo(request->message());
  }
  void setTcpServer(TcpServer * server){
	  tcpServer = server;
	  proxy = new RpcProxy(server);
  }
private:
	TcpServer * tcpServer;
	RpcProxy * proxy;
};


int main()    
{
	io_service ios;
	TcpServer serv(ios);
	EchoImplService *es = new EchoImplService();
	es->setTcpServer(&serv);
	serv.addService(es);
	boost::thread t(boost::bind(&boost::asio::io_service::run, &ios));
	std::cout<<"io complete"<<endl;
	while(true){
	}
  }
