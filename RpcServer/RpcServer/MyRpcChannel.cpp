#include "stdafx.h"
#include "MyRpcChannel.h"
#include <iostream>
#include <WINSOCK2.H> 
#include "echo.pb.h"
namespace google {
namespace protobuf {
MyRpcChannel::MyRpcChannel(TcpServer * tp)
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
							  c[0]= '0'+method->index();//这里func id有一定的数量限制。
							  c[1] = 0;
							  std::string cstr = std::string(c);
							  cstr+=request->SerializeAsString();
							  m_tcpClient->sendMessage(cstr);
							  std::cout<<"send:"<<cstr<<std::endl;
							  //done->Run();
}



}}//namespace