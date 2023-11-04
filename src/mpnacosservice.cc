#include "mpnacosservice.h"
#include <iostream>

using namespace std;

MpNacosService::MpNacosService(std::string ip, std::string port, bool isClient) : m_socket(ip + ":" + port), m_isClient(isClient) {
    m_props[PropertyKeyConst::SERVER_ADDR] = m_socket;

    // 一个单机nacos无法运行多个实例进程，因为监听端口无法同时监听不同实例，所以在此错开
    if(isClient)
    {
        m_props[PropertyKeyConst::UDP_RECEIVER_PORT] = "8998";
    }

    m_factory = NacosFactoryFactory::getNacosFactory(m_props);
    m_configService = m_factory->CreateConfigService();
    m_namingSvc = m_factory->CreateNamingService();
}

MpNacosService::~MpNacosService()
{
    ResourceGuard <INacosServiceFactory> _guardFactory(m_factory);
    ResourceGuard <ConfigService> _serviceFactoryConfig(m_configService);
    ResourceGuard <NamingService> _serviceFactoryNaming(m_namingSvc);
}

// 配置服务
bool MpNacosService::SetConfig(std::string key, std::string value) {
    bool bSucc = false;
    bSucc = m_configService->publishConfig(key, NULLSTR, value);
    return bSucc;
}

// 获取服务
std::string MpNacosService::GetConfig(std::string key) {
    std::string ss = m_configService->getConfig(key, NULLSTR, 1000);
    return ss;
}

// 发布服务
void MpNacosService::SetServiceNaming(std::string serviceName, std::string ip, 
                                    uint32_t port, const std::map<std::string, std::string>& metadata, bool ephemeral)
{
    Instance instance;
    //instance.clusterName = clusterName;
    //instance.groupName = appName;
    instance.ip = ip;
    instance.port = port;
    instance.metadata = metadata;
    //instance.instanceId = clusterName + "/" + appName + "/" + serviceName;
    instance.instanceId =serviceName;
    instance.ephemeral = ephemeral;
    
    m_namingSvc->registerInstance(serviceName, instance);
}


// 获取服务
list <Instance> MpNacosService::getAllServiceNaming(std::string& serviceName, std::string appName)
{
    list <Instance> instances = m_namingSvc->getAllInstances(serviceName, appName);
    return instances;
}

// 删除服务
void MpNacosService::delServiceNaming(std::string serviceName, std::string ip, uint16_t port)
{
    m_namingSvc->deregisterInstance(serviceName, ip, port);
}

// int main() {
//     MpNacosConfig mcf("127.0.0.1", "8848"); // 不需要使用 new 来创建对象

//     // std::cout << mcf.SetConfig("may", "1234") << std::endl;
//     // std::cout << mcf.GetConfig("may") << std::endl;

//     mcf.SetServiceNaming("DefaultCluster", "Logins", "127.0.0.1", 2312);
//     mcf.delServiceNaming("Logins", "127.0.0.1", 2312);
//     return 0;
// }