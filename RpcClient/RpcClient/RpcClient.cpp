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

void Done(){
	std::cout<<"Done"<<std::endl;
}

int main()    
{   
	//DoRPC();
	io_service ios;
	TcpClient cl(ios);
	boost::thread t(boost::bind(&boost::asio::io_service::run, &ios));


	

	RpcChannel* channel = new MyRpcChannel(&cl);
	RpcController* controller = NULL;

	// The protocol compiler generates the SearchService class based on the
	// definition given above.
	echo::EchoService* service = new echo::EchoService::Stub(channel);

	echo::EchoRequest request;
	echo::EchoResponse response;
	// Set up the request.
	request.set_message("hello world");
	// Execute the RPC.
	service->Echo(controller, &request, &response, NewCallback(&Done));
	while(true){
	}
return 0; 


}