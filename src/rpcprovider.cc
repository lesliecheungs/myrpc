#include "rpcprovider.h"
#include "mprpcapplication.h"
#include "rpcheader.pb.h"
#include "mpnacosservice.h"

/**这里是框架提供给外部使用的，可以发布rpc方法的函数接口
 * service_name => service描述
 *                         => service* 记录服务对象
*/
void RpcProvider::NotifyService(google::protobuf::Service *service)
{
    
    ServiceInfo service_info;
    
    // 获取服务对象的描述信息
    const google::protobuf::ServiceDescriptor *pserviceDesc = service->GetDescriptor();

    // 获取服务的名字
    std::string service_name = pserviceDesc->name();

    // 获取服务对象service的方法数量
    int methodCnt = pserviceDesc->method_count();

    std::cout << "----------------------------" << std::endl;
    std::cout << "提供的服务与方法： " << service_name << std::endl;
    std::cout << "service_name: " << service_name << std::endl;
    for(int i = 0; i < methodCnt; i++)
    {
        // 获取了服务对象指定下标的服务的方法的描述（抽象描述）
        const google::protobuf::MethodDescriptor* pmethodDesc = pserviceDesc->method(i);
        std::string method_name = pmethodDesc->name();
        service_info.m_methodMap.insert({method_name, pmethodDesc});
        
        std::cout << "method_name: " << method_name << std::endl;
    }
    service_info.m_service = service;
    m_serviceMap.insert({service_name, service_info});
}

// 启动rpc服务节点，开始提供rpc远程网络调用服务
void RpcProvider::Run()
{
    std::string localservername = MprpcApplication::GetInstance().GetConfig().Load("localservername");
    std::string ip = MprpcApplication::GetInstance().GetConfig().Load("localserverip");
    uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("localserverport").c_str());
    
    muduo::net::InetAddress address(ip, port);

    // 创建TcpServer对象
    muduo::net::TcpServer server(&m_eventLoop, address, localservername);

    // 绑定连接回调和消息读写回调方法
    server.setConnectionCallback(std::bind(&RpcProvider::OnConnection, this, std::placeholders::_1));
    server.setMessageCallback(std::bind(&RpcProvider::OnMessage, this, 
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    // 设置muduo库的线程数量
    server.setThreadNum(4);


    // 将本地服务上传到NacosServiceNaming
    std::cout << "RpcProvider start service at ip: " << ip << " port: " << port << std::endl;
    std::string nacosconfigip = MprpcApplication::GetInstance().GetConfig().Load("nacosserviceip");
    std::string nacosconfigport = MprpcApplication::GetInstance().GetConfig().Load("nacosserviceport");
    // std::cout << "nacosconfigip start service at ip: " << nacosconfigip << " rpcappname: " << rpcappname << std::endl;
    MpNacosService mcf(nacosconfigip, nacosconfigport);

    for(auto& sp: m_serviceMap)
    {
        std::string service_name = sp.first;
        for(auto &mp: sp.second.m_methodMap) 
        {
            std::string method_name = mp.first;
            char method_path_data[128] = {0};
            std::cout << service_name << " " << method_name << std::endl;

            mcf.SetServiceNaming(method_name, ip, port);
        }
        
    }
    

    server.start();
    m_eventLoop.loop();
    std::cout << "run end------" << std::endl;
}

// 新的socket连接回调
void RpcProvider::OnConnection(const muduo::net::TcpConnectionPtr& conn)
{
    if(!conn->connected())
    {
        conn->shutdown();
    }
}

/**
 * 已建立连接用户的读写事件回调，如果远程有一个rpc服务的调用请求，那么OnMessage方法就会响应
 * 在框架内部，rpcprovider和rpcconsumer协商好之间通信的protobuf数据类型
 * service_name method_name args
 * header_size(4字节) + header_str + args_str
*/
void RpcProvider::OnMessage(const muduo::net::TcpConnectionPtr& conn, muduo::net::Buffer* buffer, muduo::Timestamp)
{
    // 网络上接收的远程rpc调用请求的字符流 Login args
    std::string recv_buf = buffer->retrieveAllAsString();

    // 从字符流中读取前4个字节的内容
    uint32_t header_size = 0;
    recv_buf.copy((char*)&header_size, 4, 0);

    // 根据header_size读取数据头的原始字符流，反序列化数据，得到rpc请求的详细信息
    std::string rpc_header_str = recv_buf.substr(4, header_size);
    mprpc::RpcHeader rpcHeader;

    std::string service_name;
    std::string method_name;
    uint32_t args_size;
    if(rpcHeader.ParseFromString(rpc_header_str))
    {
        // 数据头反序列化成功
        service_name = rpcHeader.service_name();
        method_name = rpcHeader.method_name();
        args_size = rpcHeader.args_size();
    }
    else
    {
        // 失败
        std::cout << "rpc_header_str:" << rpc_header_str << "parse_error" << std::endl;
        return;
    }
    // 获取rpc方法参数的字符流数据
    std::string args_str = recv_buf.substr(4 + header_size, args_size);
    //打印调试信息
    std::cout << "----------------------------------------------------" << std::endl;
    std::cout << header_size << std::endl;
    std::cout << rpc_header_str << std::endl;
    std::cout << service_name << std::endl;
    std::cout << args_size << std::endl;
    std::cout << args_str << std::endl;

    // 获取service对象和method对象
    auto it = m_serviceMap.find(service_name);
    if(it == m_serviceMap.end())
    {
        std::cout << service_name << " is not exist!" << std::endl;
        return;
    }

    auto mit = it->second.m_methodMap.find(method_name);
    if(mit == it->second.m_methodMap.end())
    {
        std::cout << method_name << " is not exist!" << std::endl;
        return;
    }

    google::protobuf::Service *service = it->second.m_service; // 获取service对象
    const google::protobuf::MethodDescriptor* method = mit->second; // 获取method对象


    // 生成rpc方法调用的请求request和响应response参数
    google::protobuf::Message *request = service->GetRequestPrototype(method).New();
    if(!request->ParseFromString(args_str))
    {
        std::cout << "request parse error, content: " << args_str << std::endl;
        return;
    }

    google::protobuf::Message *response = service->GetResponsePrototype(method).New();

    // 给下面的method方法的调用，绑定一个Closure的回调函数，当被调用的方法完成后被回调
    google::protobuf::Closure *done = 
        google::protobuf::NewCallback<RpcProvider, 
                                    const muduo::net::TcpConnectionPtr&, 
                                    google::protobuf::Message*>(this, &RpcProvider::SendRpcResponse, conn, response);
    // 在框架上根据远端rpc请求，调用当前rpc节点上发布的方法
    // new UserService().Login(controller, request, response, done)
    service->CallMethod(method, nullptr, request, response, done);
}

// Closure的回调操作，用于序列化rpc的响应和网络发送
void RpcProvider::SendRpcResponse(const muduo::net::TcpConnectionPtr& conn, google::protobuf::Message* response)
{
    std::string response_str;
    if(response->SerializeToString(&response_str))
    {
        std::cout << "发送给客户端：" << response_str << std::endl;
        // 序列化成功后，通过rpc方法执行的结果发送回rpc的调用方
        conn->send(response_str);
    }
    else
    {
        std::cout << "serialize response_str error!" << std::endl;
    }
    conn->shutdown();//模拟http短连接服务，由rpcprovider主动断开连接
}