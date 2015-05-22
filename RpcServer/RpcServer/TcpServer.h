#pragma once
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <boost/asio/io_service.hpp>
#include <vector>
#include <google/protobuf/service.h>
#include "echo.pb.h"
using namespace boost::asio;
class TcpServer
{
public:
	TcpServer(io_service & io):ios(io),_sock(new ip::tcp::socket(ios)),
		acceptor(ios,ip::tcp::endpoint(ip::tcp::v4(),6688)){
		_start();
	}
	~TcpServer(void);
	void sendMessage(std::string str){
		_sock->async_write_some(buffer(str.c_str(),strlen(str.c_str())),boost::bind(&TcpServer::write_handler,this,boost::asio::placeholders::error));
	}
	//不允许使用抽象类型的参数，但是可以使用抽象类型的引用作为参数。
	//如果直接使用抽象类型按值传递，就需要构造一个新的实例。组不到
	//如果是按引用传递，那么不需要构造新的，直接对象传进来。
	void addService(google::protobuf::Service *serv){
		rpcServices.push_back(serv);
	}
private:
	io_service &ios;
	ip::tcp::acceptor acceptor;
	typedef boost::shared_ptr<ip::tcp::socket> sock_pt;
	sock_pt _sock;
	std::vector<google::protobuf::Service*> rpcServices;
	void _start(){
		acceptor.async_accept(*_sock, boost::bind(&TcpServer::accept_hander,this,boost::asio::placeholders::error));//TODO bind不懂
	}

	void write_handler(const boost::system::error_code &){
		std::cout<<"send msg complete!"<<std::endl;
	}

	//TCP链接发生时，回调函数
	void accept_hander(const boost::system::error_code & ec)
	{
		std::cout<<"client is connected!"<<std::endl;
		_sock->async_write_some(buffer("hello"),boost::bind(&TcpServer::write_handler,this,boost::asio::placeholders::error));
		boost::shared_ptr<std::vector<char>> str(new std::vector<char>(100,0));
		_sock->async_read_some(buffer(*str),boost::bind(&TcpServer::read_handler,this,boost::asio::placeholders::error,str));
	}

	//接收到数据
	void read_handler(const boost::system::error_code& ec,boost::shared_ptr<std::vector<char>> str){
		std::cout<<"read something"<<std::endl;
		if (ec){
			std::cout<<"read something err"<<std::endl;
			return;
		}
		std::cout<<"read something:"<<&(*str)[0]<<std::endl;
		deal_rpc_data(str);
		boost::shared_ptr<std::vector<char>> str2(new std::vector<char>(100,0));
		_sock->async_read_some(buffer(*str2),boost::bind(&TcpServer::read_handler,this,boost::asio::placeholders::error,str2));
	}
	void deal_rpc_data(boost::shared_ptr<std::vector<char>> str){
		echo::EchoRequest request;
		int fid = (*str)[0] - '0';
		request.ParseFromString(&(*str)[1]);
		rpcServices[0]->CallMethod(rpcServices[0]->GetDescriptor()->method(fid),NULL,&request,NULL,NULL);
	}
};

