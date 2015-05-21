#pragma once
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <iostream>
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

private:
	io_service &ios;
	ip::tcp::acceptor acceptor;
	typedef boost::shared_ptr<ip::tcp::socket> sock_pt;
	sock_pt _sock;
	void _start(){
		//sock_pt sock();
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
	void read_handler(const boost::system::error_code& ec,boost::shared_ptr<std::vector<char>> str){
		std::cout<<"read something"<<std::endl;
		if (ec){
			std::cout<<"read something err"<<std::endl;
			return;
		}
		std::cout<<"read something:"<<&(*str)[0]<<std::endl;
		boost::shared_ptr<std::vector<char>> str2(new std::vector<char>(100,0));
		_sock->async_read_some(buffer(*str2),boost::bind(&TcpServer::read_handler,this,boost::asio::placeholders::error,str2));
	}
	

	
};

