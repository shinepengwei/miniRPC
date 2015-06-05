#pragma once
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio/io_service.hpp>
#include <vector>
#include <google/protobuf/service.h>


using namespace boost::asio;
typedef boost::asio::ip::tcp::socket* sock_pt;
class TcpConnection:
	public google::protobuf::RpcChannel
{
public:
	TcpConnection(boost::asio::io_service & io);
	~TcpConnection();
	//发送数据
	void sendMessage(std::string str);

	//发送数据回调
	void write_handler(const boost::system::error_code &);
	//接收到数据
	void read_handler(const boost::system::error_code& ec,boost::shared_ptr<std::vector<char>> str);
	
	//rpc server service
	void addService(google::protobuf::Service *serv);
	sock_pt getSocket();


void CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller,
                          const google::protobuf::Message* request,
                          google::protobuf::Message* response,
                          google::protobuf::Closure* done);

private:
	sock_pt _sock;
	std::vector<google::protobuf::Service*> rpcServices;
	//解析rpc string。
	void deal_rpc_data(boost::shared_ptr<std::vector<char>> str);
};




class TcpServer
{
public:
	TcpServer(boost::asio::io_service & io);
	void sendMessageToAllClient(std::string str);
	void echo(std::string str);
private:
	boost::asio::ip::tcp::acceptor acceptor;
	std::vector<TcpConnection *> m_cons;//连接
	TcpConnection * m_waitCon;
	boost::asio::io_service * m_ios;

	void _start();

	//TCP链接发生时，回调函数
	void accept_hander(const boost::system::error_code & ec);
};

class TcpClient
{
public:
	TcpClient(io_service & io);
	TcpConnection * getConnection();
private:
	TcpConnection * m_con;
	ip::tcp::endpoint ep;
	//boost::asio::io_service m_ios;
	void conn_hanlder(const boost::system::error_code & ec,TcpConnection * con);
	
};