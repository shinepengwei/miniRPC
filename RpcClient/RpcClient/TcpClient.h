#pragma once
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <vector>
using namespace boost::asio;

class TcpClient
{
	
public:
	TcpClient(io_service &io):ios(io),_sock(new ip::tcp::socket(ios)),ep(ip::address::from_string("127.0.0.1"),6688){
		//sock_pt sock(new ip::tcp::socket(ios));
		_sock->async_connect(ep,boost::bind(&TcpClient::conn_hanlder,this,boost::asio::placeholders::error));
	}
	~TcpClient(void);
	//发送一个字符串
	void sendMessage(std::string str){
		_sock->async_write_some(buffer(str.c_str(),strlen(str.c_str())),boost::bind(&TcpClient::write_handler,this,boost::asio::placeholders::error));
		//_sock->async_write_some(buffer(str),boost::bind(&TcpClient::write_handler,this,boost::asio::placeholders::error));
	}

private:
	io_service &ios;
	ip::tcp::endpoint ep;
	typedef boost::shared_ptr<ip::tcp::socket> sock_pt;
	sock_pt _sock;


	//连接成功回调函数
	void conn_hanlder(const boost::system::error_code & ec){
		if (ec){
			std::cout<<"connect failed"<<std::endl;
			return;
		}
		boost::shared_ptr<std::vector<char>> str(new std::vector<char>(100,0));
		std::cout<<"connect success"<<std::endl;
		_sock->async_read_some(buffer(*str),boost::bind(&TcpClient::read_handler,this,boost::asio::placeholders::error,str));
	}

	//接受信息回调
	void read_handler(const boost::system::error_code& ec,boost::shared_ptr<std::vector<char>> str){
		std::cout<<"read something"<<std::endl;
		if (ec){
			std::cout<<"read something err"<<std::endl;
			return;
		}

		std::cout<<"read something:"<<&(*str)[0]<<std::endl;
		boost::shared_ptr<std::vector<char>> str2(new std::vector<char>(100,0));
		_sock->async_read_some(buffer(*str2),boost::bind(&TcpClient::read_handler,this,boost::asio::placeholders::error,str2));
	}

	//发送信息成功回调
	void write_handler(const boost::system::error_code &){
		std::cout<<"send msg complete!"<<std::endl;
	}
};

