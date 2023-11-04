#include <iostream>
#include "mprpcapplication.h"
#include "entrywebserver.pb.h"
#include "mprpcchannel.h"

/**
 * 这是客户端借助mprpc框架调用远端服务demo
*/
int main(int argc, char **argv)
{
    // 整个程序启动以后，想使用mprpc框架来享受rpc服务调用，一定需要先调用框架的初始化函数
    MprpcApplication::Init(argc, argv);

    // 演示调用远程发布的rpc方法Login
    fixbug::EntryWebServer_Stub stub(new MprpcChannel());
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");

    fixbug::LoginResponse response;

    // 发起rpc方法的调用， 同步的rpc调用过程
    MprpcController controller;
    stub.Login(&controller, &request, &response, nullptr); // RpcChannel->RpcChannel::callMethod 集中来做所有rpc方法调用的参数序列化和网络发送
    // 一次rpc调用完成，读调用的结果
    if (0 == response.result().errcode())
    {
        // 注意success，是在proto文件中定义的response的成员变量
        std::cout << "rpc login response success:" << response.success() << std::endl;
        
    }
    else
    {
        std::cout << "rpc login response error : " << response.result().errmsg() << std::endl;
    }
    if(controller.Failed()) {
        exit(EXIT_FAILURE);
    }

    
    // 演示调用远程发布的rpc方法Register
    fixbug::RegisterRequest Rerequest;
    Rerequest.set_id(1);
    Rerequest.set_name("li si");
    Rerequest.set_pwd("absdqw");

    fixbug::RegisterResponse Reresponse;

    // 发起rpc方法的调用， 同步的rpc调用过程
    stub.Register(nullptr, &Rerequest, &Reresponse, nullptr); // RpcChannel->RpcChannel::callMethod 集中来做所有rpc方法调用的参数序列化和网络发送
    
    if(controller.Failed()) {
        exit(EXIT_FAILURE);
    }
    if (0 == response.result().errcode())
    {
        std::cout << "rpc reg response success:" << response.success() << std::endl;
    }
    else
    {
        std::cout << "rpc reg response error : " << response.result().errmsg() << std::endl;
    }
    return 0;
}