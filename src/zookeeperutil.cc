#include "zookeeperutil.h"
#include "mprpcapplication.h"
#include <semaphore.h>
#include <iostream>

// 全局的watcher观察器 zkserver给zkclient的通知
void global_watcher(zhandle_t* zh, int type, int state, const char* path, void* watcherCtx)
{
    if(type == ZOO_SESSION_EVENT) // 回调的消息类型是和会话相关的消息类型
    {
        if(state == ZOO_CONNECTED_STATE) // zkclient和zkserver连接成功
        {
            sem_t* sem = (sem_t*)zoo_get_context(zh);
            sem_post(sem);
        }
    }
}

ZkClient::ZkClient():m_zhandle(nullptr)
{
}

ZkClient::~ZkClient() {
    if(m_zhandle != nullptr)
    {
        zookeeper_close(m_zhandle);
    }
}

// 连接zkserver
void ZkClient::start()
{
    std::string host = MprpcApplication::GetInstance().GetConfig().Load("zookeeperip");
    std::string port = MprpcApplication::GetInstance().GetConfig().Load("zookeeperport");
    std::string connstr = host + ":" + port;

    /**
     * zookeeper_mt: 多线程版本
     * zookeeper的API客户端程序提供了三个线程
     * API调用线程
     * 网络I/O线程 pthread_create poll
     * watcher回调线程 pthread_create
    */
    // 此时还是句柄的初始化，连接是否成功另说
    m_zhandle = zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr, nullptr, 0);
    if(nullptr == m_zhandle)
    {
        std::cout << "zookeeper_init error!" << std::endl;
        exit(EXIT_FAILURE);
    }

    sem_t sem;
    sem_init(&sem, 0, 0);
    zoo_set_context(m_zhandle, &sem);
    // 此时p是为了等待zk连接成功的通知，通知成功（在global_watcher接收）则会进行v操作
    sem_wait(&sem);
    std::cout << "zookeeper_init success" << std::endl;
}

// 向zkserver创建节点
void ZkClient::create(const char* path, const char* data, int datalen, int state)
{
    char path_buffer[128] = {0};
    int bufferlen = sizeof path_buffer;
    // 先判断该节点是否存在
    int flag = zoo_exists(m_zhandle, path, 0, nullptr);
    if(ZNONODE == flag)
    {
        flag = zoo_create(m_zhandle, path, data, datalen, &ZOO_OPEN_ACL_UNSAFE, state, path_buffer, bufferlen);
        if(flag == ZOK)
        {
            std::cout << "znode create success ... path: " << path << std::endl; 
        }
        else 
        {
            std::cout << "flag:" << flag << std::endl;
            std::cout << "znode create error... ptah: " << path << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

// 根据指定的path，获取znode节点的值
std::string ZkClient::GetData(const char* path)
{
    char buffer[64] = {0};
    int bufflen = sizeof buffer;
    int flag = zoo_get(m_zhandle, path, 0, buffer, &bufflen, nullptr);
    if(flag != ZOK)
    {
        std::cout << "get znode error ... path: " << path << std::endl;
        return "";
    }

    return buffer;
}