#include <iostream>
#include <string>
#include "../user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"

class UserService : public fixbug::UserServiceRpc
{
public:
    bool Login(std::string name, std::string pwd)
    {
        std:: cout << "doing local service: Login" << std::endl;
        std::cout << "name: " << name << " pwd: " << pwd << std::endl;
        return true;
    }

    bool Register(uint32_t id, std::string name, std::string pwd)
    {
        std::cout << "doing local service: Register" << std::endl;
        std::cout << "id: " << id << " name: " << name << " pwd: " << pwd << std::endl;
        return true; 
    }

    /*
        重写基类UserServiceRpc的虚函数，下面这些方法都是框架直接调用的
        1. caller ===> Login(LoginRequest) => muduo => callee
        2. callee ===> Login(LoginRequest) => 交到下面这个重写的Login方法
    */
    void Login(::google::protobuf::RpcController* controller,
                         const ::fixbug::LoginRequest* request,
                         ::fixbug::LoginResponse* response,
                         ::google::protobuf::Closure* done) 
    {
        // 1.框架给业务上报了请求参数LoginRequest，业务获取相应数据作本地业务
        std::string name = request->name();
        std::string pwd = request->pwd();

        // 2.作本地业务
        bool login_result = Login(name, pwd);

        // 3.把响应写入
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(login_result);

        // 4.执行回调操作
        done->Run();
    }

    void Register(::google::protobuf::RpcController* controller,
                         const ::fixbug::RegisterRequest* request,
                         ::fixbug::RegisterResponse* response,
                         ::google::protobuf::Closure* done) 
    {
        uint32_t id = request->id();
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool register_result = Register(id, name, pwd);

        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(register_result);

        done->Run();
    }

};

int main(int argc, char **argv) {
    // 框架的初始化工作
    MprpcApplication::Init(argc, argv);

    // provider是一个rpc网络服务对象，把userservice对象发布到rpc节点上
    RpcProvider provider;
    provider.NotifyService(new UserService());
    
    // 启动一个rpc服务节点 Run以后，进程进入阻塞模式，等待远方调用
    provider.Run();
    return 0;
}