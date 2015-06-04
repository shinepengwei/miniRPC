#pragma once
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include <vector>
#include <google/protobuf/service.h>

using namespace boost::asio;
typedef boost::shared_ptr<boost::asio::ip::tcp::socket> sock_pt;
class TcpConnection
{
public:
	TcpConnection();
	~TcpConnection();

private:

};



class TcpEntity{
public:
	TcpEntity(boost::asio::io_service &io);
	//发送数据
	void sendMessage(std::string str);
	//发送数据回调
	void write_handler(const boost::system::error_code &);
	//接收到数据
	void read_handler(const boost::system::error_code& ec,boost::shared_ptr<std::vector<char>> str);
	
	//rpc server service
	void addService(google::protobuf::Service *serv);

protected:
	boost::asio::io_service &ios;
	std::vector<google::protobuf::Service*> rpcServices;
	
	sock_pt _sock;

	void deal_rpc_data(boost::shared_ptr<std::vector<char>> str);
};


class TcpServer:public TcpEntity
{
public:
	TcpServer(boost::asio::io_service & io);

private:
	boost::asio::ip::tcp::acceptor acceptor;
	void _start();

	//TCP链接发生时，回调函数
	void accept_hander(const boost::system::error_code & ec);
};

class TcpClient:public TcpEntity{
public:
	TcpClient(io_service & io);
	
private:
	ip::tcp::endpoint ep;
	void conn_hanlder(const boost::system::error_code & ec,sock_pt sock);
};