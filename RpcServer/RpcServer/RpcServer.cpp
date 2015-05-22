// RpcServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
/*    
 * testSocketService.c    
 *    
 *  Created on: 2012-8-16    
 *      Author: 皓月繁星    
 */
#include <WINSOCK2.H> 
#include <iostream>
#include <stdio.h>   
#include "echo.pb.h"
#include "TcpServer.h"
#include <boost/thread/thread.hpp>       
#define PORT           5150  
#define MSGSIZE        1024  
                 
#pragma comment(lib, "ws2_32.lib")
             
using namespace google::protobuf;
class EchoImplService : public echo::EchoService {
 public:
  void Echo(RpcController* controller,
              const echo::EchoRequest* request,
              echo::EchoResponse* response,
              Closure* done) {
				  std::cout<<"RPC,message:"<<request->message()<<std::endl;
     
    //done->Run();
  }
};


int main()    
{    /*

	 EchoImplService *service = new EchoImplService(); 
    while (TRUE) {    
        ret = recv(sClient, szMessage, MSGSIZE, 0);
		if (ret > 0){
			echo::EchoRequest request;
			int fid = szMessage[0] - '0';
			request.ParseFromString(szMessage+1);
			service->CallMethod(service->GetDescriptor()->method(fid),NULL,&request,NULL,NULL);
			szMessage[ret] = '\0';    
			printf("Received [%d bytes]: '%s'\n", ret, szMessage); 
		}
    }    
    return 0;    
	*/
	io_service ios;
	TcpServer serv(ios);
	EchoImplService *service = new EchoImplService(); 
	serv.addService(service);
	boost::thread t(boost::bind(&boost::asio::io_service::run, &ios));
	std::cout<<"io complete"<<endl;
	while(true){
	}
  }
