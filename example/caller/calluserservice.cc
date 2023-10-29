#include <iostream>
#include "mprpcapplication.h"
#include "user.pb.h"
#include "mprpcchannel.h"

int main(int argc, char **argv)
{
    // 整个程序启动以后，想使用mprpc框架来享受rpc服务调用，一定需要先调用框架的初始化函数
    MprpcApplication::Init(argc, argv);

    // 演示调用远程发布的rpc方法Login
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");

    fixbug::LoginResponse response;

    // 发起rpc方法的调用， 同步的rpc调用过程
    MprpcController controller;
    stub.Login(&controller, &request, &response, nullptr); // RpcChannel->RpcChannel::callMethod 集中来做所有rpc方法调用的参数序列化和网络发送
    
    if(controller.Failed()) {
        exit(EXIT_FAILURE);
    }


    // 演示调用远程发布的rpc方法Login
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
    
    return 0;
}