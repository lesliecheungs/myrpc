#pragma once

#include <string>
#include <memory>
#include <unistd.h>
#include <list>
#include "Nacos.h"

using namespace nacos;

class MpNacosService
{
public:
    MpNacosService();
    MpNacosService(std::string ip, std::string port, bool isClient = false);
    ~MpNacosService();

    bool SetConfig(std::string key, std::string value); // 配置文件
    std::string GetConfig(std::string key); // 获取配置文件

    // 发布服务
    void SetServiceNaming(std::string serviceName, std::string ip, 
                                    uint32_t port, const std::map<std::string, std::string>& metadata = {}, bool ephemeral = true);                    
    std::list <Instance> getAllServiceNaming(std::string& serviceName, std::string appName = "DEFAULT_GROUP"); // 获取所有服务
    void delServiceNaming(std::string serviceName, std::string ip, uint16_t port); // 删除服务

private:
    std::string m_socket;   
    Properties m_props;
    INacosServiceFactory* m_factory;
    ConfigService* m_configService;
    NamingService* m_namingSvc;
    bool m_isClient;
};