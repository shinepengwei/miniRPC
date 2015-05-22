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
class EchoImplService : public echo::EchoService {
 public:
  void Echo(RpcController* controller,
              const echo::EchoRequest* request,
              echo::EchoResponse* response,
              Closure* done) {
				  std::cout<<"RPC,message:"<<request->message()<<std::endl;
  }
};


int main()    
{   
	//DoRPC();
	io_service ios;
	TcpClient cl(ios);
	cl.addService(new EchoImplService());
	boost::thread t(boost::bind(&boost::asio::io_service::run, &ios));
	RpcChannel* channel = new MyRpcChannel(&cl);
	echo::EchoService* service = new echo::EchoService::Stub(channel);
	echo::EchoRequest request;
	request.set_message("hello world");
	service->Echo(NULL, &request, NULL, NULL);
	while(true){
	}
return 0; 


}