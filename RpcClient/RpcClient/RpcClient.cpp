/*    
 * testSocketClient.c    
 *    
 *  Created on: 2012-8-16    
 *      Author: ð©ÔÂ·±ÐÇ    
 */
#include "stdafx.h"
#include "TcpClient.h"
#include <stdio.h>   
#include <iostream>
#include  <google\protobuf\service.h>
#include <boost/thread/thread.hpp>       

#include <boost/lexical_cast.hpp>     
#include <google\protobuf\service.h>
#include "echo.pb.h"
#include "MyRpcChannel.h"
using namespace google::protobuf;

RpcChannel* channel;
RpcController* controller;
echo::EchoService* service;
echo::EchoRequest request;
echo::EchoResponse response;
void Done() {
	std::cout<<"DOne"<<std::endl;
  delete service;
  delete channel;
  delete controller;
}
void DoRPC() {
	
  // You provide classes MyRpcChannel and MyRpcController, which implement
  // the abstract interfaces protobuf::RpcChannel and protobuf::RpcController.
  channel = new MyRpcChannel();
  controller = NULL;

  // The protocol compiler generates the SearchService class based on the
  // definition given above.
  service = new echo::EchoService::Stub(channel);

  // Set up the request.
  request.set_message("hello world");
  // Execute the RPC.
  service->Echo(controller, &request, &response, NewCallback(&Done));
}


int main()    
{   
	//DoRPC();
	io_service ios;
	TcpClient cl(ios);
	boost::thread t(boost::bind(&boost::asio::io_service::run, &ios));
	std::cout<<"io complete"<<endl;
	while (true)
	{
		//ios.poll();
		char szMessage[100]; 
		gets_s(szMessage); 
		cl.sendMessage(szMessage);
	}


return 0; 


}