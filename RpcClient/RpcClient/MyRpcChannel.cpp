#include "stdafx.h"
#include "MyRpcChannel.h"
#include <iostream>
#include <WINSOCK2.H> 
#include "echo.pb.h"
namespace google {
namespace protobuf {

	//定义程序中使用的常量    
#define SERVER_ADDRESS "127.0.0.1" //服务器端IP地址    
#define PORT           5150         //服务器的端口号    
#define MSGSIZE        1024         //收发缓冲区的大小    
#pragma comment(lib, "ws2_32.lib")    

MyRpcChannel::MyRpcChannel(TcpClient * tp)
{
	 m_tcpClient = tp;
}


MyRpcChannel::~MyRpcChannel(void)
{
}

void MyRpcChannel::CallMethod(const MethodDescriptor* method,
                          RpcController* controller,
                          const Message* request,
                          Message* response,
						  Closure* done){
							  char c[2];
							  c[0]= '0'+method->index();
							  c[1] = 0;
							  std::string cstr = std::string(c);
							  cstr+=request->SerializeAsString();
							  m_tcpClient->sendMessage(cstr);
							  std::cout<<"send:"<<cstr<<std::endl;
}



}}//namespace