// RpcServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "echo.pb.h"
#include "TcpConnect.h"
#include <boost/thread/thread.hpp>   
#include "RPCServerService.h"

using namespace boost::asio;

int main()    
{
	io_service ios;
	int inPara = 0;
	std::cout<<"0:server,    other:client"<<std::endl;
	std::cin>>inPara;
	if (inPara == 0){
		TcpServer *server = new TcpServer(ios);
		//boost::thread t(boost::bind(&boost::asio::io_service::run, &ios));
		//while(true){
		//	ios.poll();
		//}
		ios.run();
	}else{
		TcpClient *client = new TcpClient(ios);
		boost::thread t(boost::bind(&boost::asio::io_service::run, &ios));
		RpcProxy *proxy = new RpcProxy(client);
		EchoImplService *es = new EchoImplService(client);
		while(true){
			//ios.poll();
			string in;
			std::cin>>in;
			echo::EchoRequest request;
			request.set_message(in);
			echo::EchoService * service = new echo::EchoService::Stub(m_channel);
			service->Echo(NULL, &request, NULL, NULL);

			}
	}
  }
