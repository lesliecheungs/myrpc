#pragma once

#include <semaphore.h>
#include <zookeeper/zookeeper.h>
#include <string>

class ZkClient
{
public:
    ZkClient();
    ~ZkClient();
    void start();
    // state表示创建的是否是临时性节点
    // 1代表临时性节点：rpc节点超时未发送心跳消息 则自动删除该节点
    // 0代表永久性节点：。。。。。。。。。。。。。。不会删除该节点
    void create(const char* path, const char* data, int datalen, int state = 0);
    std::string GetData(const char* path);
private:
    //  zk的客户端句柄
    zhandle_t* m_zhandle;
};