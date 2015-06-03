// RpcServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "echo.pb.h"
#include "TcpConnect.h"
#include <boost/thread/thread.hpp>   
#include "rpcProxy.h"
#include "RPCServerService.h"

using namespace boost::asio;

int main()    
{
	io_service ios;
	int inPara = 0;
	std::cout<<"0:server,    other:client"<<std::endl;
	std::cin>>inPara;
	if (inPara == 0){
		TcpServer *con = new TcpServer(ios);
		RpcProxy *proxy = new RpcProxy(con);
		EchoBackImplService *es = new EchoBackImplService(con,proxy);
		boost::thread t(boost::bind(&boost::asio::io_service::run, &ios));
		std::cout<<"io complete"<<endl;
	}else{
		TcpClient *con = new TcpClient(ios);
		boost::thread t(boost::bind(&boost::asio::io_service::run, &ios));
		RpcProxy *proxy = new RpcProxy(con);
		EchoImplService *es = new EchoImplService(con,proxy);
		while(true){
			string in;
			std::cin>>in;
			proxy->Echo(in);
		}

		
	}

	while(true){
	}
  }
