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
    WSADATA wsaData;    
    SOCKET sListen;    
    SOCKET sClient;    
    SOCKADDR_IN local;    
    SOCKADDR_IN client;    
    char szMessage[MSGSIZE];    
    int ret;    
    int iaddrSize = sizeof(SOCKADDR_IN);    
    WSAStartup(0x0202, &wsaData);    
                 
    sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);    
                 
    local.sin_family = AF_INET;    
    local.sin_port = htons(PORT);    
    local.sin_addr.s_addr = htonl(INADDR_ANY);    
    bind(sListen, (struct sockaddr *) &local, sizeof(SOCKADDR_IN));    
                 
    listen(sListen, 1);    
                 
    sClient = accept(sListen, (struct sockaddr *) &client, &iaddrSize);    
    printf("Accepted client:%s:%d\n", inet_ntoa(client.sin_addr),    
            ntohs(client.sin_port));    
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
	boost::thread t(boost::bind(&boost::asio::io_service::run, &ios));
	std::cout<<"io complete"<<endl;
	while (true)
	{
		char szMessage[100]; 
		gets_s(szMessage); 
		serv.sendMessage(szMessage);
	}

  }
