#include "stdafx.h"
#include "TcpConnect.h"
#include <boost/bind.hpp>
#include <iostream>
#include "RPCServerService.h"


TcpConnection::TcpConnection(boost::asio::io_service &io):_sock(new boost::asio::ip::tcp::socket(io))
{
}

TcpConnection::~TcpConnection()
{
}

void TcpConnection::sendMessage(std::string str){
	std::cout<<"sending:"<<str<<std::endl;
	_sock->async_write_some(boost::asio::buffer(str.c_str(),strlen(str.c_str())),boost::bind(&TcpConnection::write_handler,this,boost::asio::placeholders::error));
}

void TcpConnection::write_handler(const boost::system::error_code &){
	std::cout<<"send msg complete!"<<std::endl;
}

void TcpConnection::read_handler(const boost::system::error_code& ec,boost::shared_ptr<std::vector<char>> str){
	if (ec){
		std::cout<<"read something err"<<std::endl;
		return;
	}
	std::cout<<"read something:"<<&(*str)[0]<<std::endl;
	deal_rpc_data(str);
	boost::shared_ptr<std::vector<char>> str2(new std::vector<char>(100,0));
	_sock->async_read_some(boost::asio::buffer(*str2),boost::bind(&TcpConnection::read_handler,this,boost::asio::placeholders::error,str2));
}
//rpc server service
void TcpConnection::addService(google::protobuf::Service *serv){
	//不允许使用抽象类型的参数，但是可以使用抽象类型的引用作为参数。
	//如果直接使用抽象类型按值传递，就需要构造一个新的实例。组不到
	//如果是按引用传递，那么不需要构造新的，直接对象传进来。
	rpcServices.push_back(serv);
}

void TcpConnection::deal_rpc_data(boost::shared_ptr<std::vector<char>> str){
	echo::EchoRequest request;
	int fid = (*str)[0] - '0';
	request.ParseFromString(&(*str)[1]);
	rpcServices[0]->CallMethod(rpcServices[0]->GetDescriptor()->method(fid),NULL,&request,NULL,NULL);
}
sock_pt TcpConnection::getSocket(){
	return _sock;
}


void TcpConnection::CallMethod(const MethodDescriptor* method,
                          RpcController* controller,
                          const Message* request,
                          Message* response,
						  Closure* done){
							  char c[2];
							  c[0]= '0'+method->index();//这里func id有一定的数量限制。
							  c[1] = 0;
							  std::string cstr = std::string(c);
							  cstr+=request->SerializeAsString();
							  sendMessage(cstr);
							  //done->Run();
}


TcpServer::TcpServer(boost::asio::io_service & io):m_ios(io),acceptor(io,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),6688)){
	_start();
	m_waitCon = NULL;
}

void TcpServer::_start(){
	TcpConnection * t_con = new TcpConnection(m_ios);
	t_con->addService(new EchoBackImplService(t_con));//con有service的句柄。
	m_waitCon = t_con;
	//目前只能接受一次连接
	acceptor.async_accept(*t_con->getSocket(), boost::bind(&TcpServer::accept_hander,this,boost::asio::placeholders::error));//TODO bind不懂
}

//TCP链接发生时，回调函数
void TcpServer::accept_hander(const boost::system::error_code & ec)
{
	std::cout<<"client is connected!"<<std::endl;
	boost::shared_ptr<std::vector<char>> str(new std::vector<char>(100,0));
	m_waitCon->getSocket()->async_read_some(boost::asio::buffer(*str),boost::bind(&TcpConnection::read_handler,m_waitCon->getSocket(),boost::asio::placeholders::error,str));
	_start();
	//TODO 监听新的连接，需要创建一个新的socket
	//acceptor.async_accept(*_sock, boost::bind(&TcpServer::accept_hander,this,boost::asio::placeholders::error));
}

TcpClient::TcpClient(boost::asio::io_service & io):m_con(new TcpConnection(io)),ep(ip::address::from_string("127.0.0.1"),6688){
	//sock_pt sock(new ip::tcp::socket(ios));
	m_con->getSocket()->async_connect(ep,boost::bind(&TcpClient::conn_hanlder,this,boost::asio::placeholders::error,m_con->getSocket()));
}
//连接成功回调函数
void TcpClient::conn_hanlder(const boost::system::error_code & ec,sock_pt sock){
	if (ec){
		std::cout<<"connect failed"<<std::endl;
		return;
	}
	boost::shared_ptr<std::vector<char>> str(new std::vector<char>(100,0));
	std::cout<<"connect success"<<std::endl;
	sock->async_read_some(buffer(*str),boost::bind(&TcpConnection::read_handler,sock,boost::asio::placeholders::error,str));
}