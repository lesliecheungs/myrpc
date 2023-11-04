#include "mprpcchannel.h"
#include "mprpcapplication.h"
#include "rpcheader.pb.h"
#include "mpnacosservice.h"

#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <list>
#include <iostream>

/**
 * 所有通过stub代理对象调用的rpc方法，都走到这里了，统一作rpc方法调用的数据序列化和网络发送
 * header_size + service_name + method_name args_size + args
*/
void MprpcChannel::CallMethod(const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller, 
                          const google::protobuf::Message* request,
                          google::protobuf::Message* response, 
                          google::protobuf::Closure* done) 
{
    const google::protobuf::ServiceDescriptor* sd = method->service();
    std::string service_name = sd->name(); //service_name
    std::string method_name = method->name(); // method_name

    // 获取参数的序列化字符串长度 args_size
    uint32_t args_size = 0;
    std::string args_str;
    if(request->SerializeToString(&args_str))
    {
        args_size = args_str.size();
    }
    else
    {
        controller->SetFailed("serialize request error !");
        return;
    }

    // 定义rpc的请求header
    mprpc::RpcHeader rpcHeader;
    rpcHeader.set_service_name(service_name);
    rpcHeader.set_method_name(method_name);
    rpcHeader.set_args_size(args_size);

    uint32_t header_size = 0;
    std::string rpc_header_str;
    if(rpcHeader.SerializeToString(&rpc_header_str))
    {
        header_size = rpc_header_str.size();
    }
    else
    {
        controller->SetFailed("serialize rpc header error!");
        return;
    }

    // 组织待发送的rpc请求的字符串
    std::string send_rpc_str;
    send_rpc_str.insert(0, std::string((char*)&header_size, 4)); // 以字节的方式写入
    send_rpc_str += rpc_header_str;
    send_rpc_str += args_str; 

    //打印调试信息
    std::cout << "----------------------------------------------------" << std::endl;
    std::cout << header_size << std::endl;
    std::cout << rpc_header_str << std::endl;
    std::cout << service_name << std::endl;
    std::cout << args_size << std::endl;
    std::cout << args_str << std::endl;

    // 因为这是客户端无需高并发，所以使用tcp编程进行发送即可, 完成rpc的远程调用即可
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientfd == -1)
    {
        char errtxt[512] = {0};
        sprintf(errtxt, "create socket error! errno:%d", errno);

        controller->SetFailed(errtxt);
        return;
    }

    // ------此处通过nacosConfig读取配置信息
    // std::string ip = MprpcApplication::GetInstance().GetConfig().Load("rpcserverip");
    // uint16_t port = atoi(MprpcApplication::GetInstance().GetConfig().Load("rpcserverport").c_str());

    std::string nacosconfigip = MprpcApplication::GetInstance().GetConfig().Load("nacosconfigip");
    std::string nacosconfigport = MprpcApplication::GetInstance().GetConfig().Load("nacosconfigport");
    std::cout << nacosconfigip << " " << nacosconfigport << std::endl;
    MpNacosService mcf(nacosconfigip, nacosconfigport, true);

    std::list <Instance> instances = mcf.getAllServiceNaming(method_name);

    std::cout << "----------------------------------------------------" << std::endl;
    std::string ip = instances.begin()->ip;
    uint16_t port = instances.begin()->port;
    std::cout << "server ip: " << ip << " server port: " << port << std::endl;
    // -----------------------------------------------------
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    // 连接rpc服务节点
    if(connect(clientfd, (struct sockaddr*)&server_addr, sizeof server_addr) == -1)
    {
        close(clientfd);
        char errtxt[512] = {0};
        sprintf(errtxt, "connect error! errno:%d", errno);

        controller->SetFailed(errtxt);
        return;
    }

    // 发送rpc请求
    if(send(clientfd, send_rpc_str.c_str(), send_rpc_str.size(), 0) == -1)
    {
        close(clientfd);
        char errtxt[512] = {0};
        sprintf(errtxt, "send error! errno:%d", errno);

        controller->SetFailed(errtxt);
        return;
    }

    // 接收rpc请求的响应值
    char recv_buf[1024] = {0};
    int recv_size = 0;
    if((recv_size = recv(clientfd, recv_buf, 1024, 0)) == -1)
    {
        close(clientfd);
        char errtxt[512] = {0};
        sprintf(errtxt, "recv error! errno:%d", errno);

        controller->SetFailed(errtxt);
        return;
    }
    
    // 反序列化rpc调用的响应数据
    // std::string response_str(recv_buf, 0, recv_size); // 存在/0问题
    //if(!response->ParseFromString(response_str))
    
    if(!response->ParseFromArray(recv_buf, recv_size))
    {
        close(clientfd);
        char errtxt[512] = {0};
        sprintf(errtxt, "recv error! errno:%d", errno);

        controller->SetFailed(errtxt);
        return;
    }
    

    close(clientfd);
}