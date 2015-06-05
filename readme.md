基于boost.asio网络模块和protobuf通信描述的RPC实现
==

##1 RPC实现框架
目前实现的RPC只能一端的RPC client调用另一端的RPC service，没有返回值。如果需要返回值，另一端调用这一端的RPC，以参数作为返回值。

RPC分为client和service，client以protobuf的stub/channel形式实现，rpc server以service实现。

网络通信的服务端和客户端，都包括了RPC服务端和RPC客户端，RPC的客户端通过网络通信调用RPC服务端。

注意区分**网络通信的服务端/客户端**与**RPC服务端/客户端**，在本文中代表两种含义。

##2 模块构成：
TcpConnection：

- RpcChannel子类，提供CallMethod函数。该函数用于RPC客户端调用stub的service时使用，包括function和参数信息的序列化，以及调用数据传输。
- 封装socket，以及和socket有关的数据传输函数。包括发送数据、接收数据回调。
- 此外，他持有RPCServerService，RPC客户端接收数据后调用RPCServerService的rpc函数。

TcpServer：

- 网络通信服务端，处理网络连接请求。
- 持有TcpConnection的vector，将所有的网络连接存放在里面。
- 每次有网络连接请求时，已连接的网络连接存起来，创建一个新的网络连接等待请求。


TcpClient:

- 只有一个网络连接。

Service分为两种：用于服务端的RPCServerService（包括EchoImplService和EchoBackImplService），以及用于客户端的echo::EchoService::Stub。

RPCServerService：

- EchoService的子类，用于实现echo的rpc被调用函数。
- 存放在TcpConnection中，当接收到数据时，TcpConnection调用此service。`rpcServices[0]->CallMethod(rpcServices[0]->GetDescriptor()->method(fid),NULL,&request,NULL,NULL);`

## RPC调用过程

####服务端初始化过程：

1 使用io_service创建TcpServer：
- 创建TcpConnection等待连接
- 添加RPC serverService当接收到数据时，调用rpc函数。（RPC服务端）

2 创建一个新线程，io_service.run()

####客户端初始化过程：

1 使用io_service创建TcpClient：
- 创建一个TcpConnection连接服务器。
- 添加RPC serverService当接收到数据时，调用rpc函数。（RPC服务端）

2 创建一个新线程，io_service.run()


####客户端发起RPC调用步骤：
1.使用TcpClient的TcpConnection作为RpcChannel创建一个echo::EchoService::Stub。
2.设置request信息，调用Stub的Echo函数。`service->Echo(NULL, &request, NULL, NULL);`
3.调用RpcChannel(TcpConnection)的Callmethod函数。此函数中对函数信息和参数信息序列化，并发到服务端。
4.服务端接收到数据，TcpConnection持有一个RPCServerService，并调用RPCServerService的CallMethod函数。
5.RpcServerService的Echo函数被调用（protoBuf中callMethod根据function信息调用Echo函数）。
6.EchoBackImplService的Echo函数调用echo::EchoService::Stub的Echo函数，开始Web客户端的RPC调用。

####服务端发起RPC调用步骤：
TcpServer有一个Echo函数，这个函数基于所有的TcpConnection创建一个EchoService::Stub，然后调用它的Echo函数。


##3 基于protobuf实现RPC流程

参考：
http://blog.csdn.net/kevinlynx/article/details/39379957

其中有个问题：如何标示service和method。文中的方法太复杂。

我的方法是，使用serviceId和functionId，将其放在要传输的字符串首部，到了服务端进行解析即可。

目前，我只是用了functionId。放在网络传输字符串的第一个字节。


ProtoBuf RPC总结：

RPC客户端调用RPC服务端，重要的是实现一个RpcChannel以及它的一个CallMethod函数，此函数用于序列化函数和函数参数信息。

RPC服务端重要的是实现EchoService的子类，也就是RPC被调用函数的具体实现。

实现了这两个东西，就是把序列化信息传来传去的过程。

##4 Boost.Asio相关问题
http://www.codingart.info//rpc-boost-asio.html


