#include "mprpcconfig.h"

#include <iostream>
#include <string>

// 加载配置文件并且保存到m_configMap
void MprpcConfig::LoadConfigFile(const char* config_file)
{
    // 1. 先通过本地文件加载nacos的基本配置
    FILE *pf = fopen(config_file, "r");
    if(pf == nullptr)
    {
        std::cout << config_file << " is note exist!" << std::endl;
        exit(EXIT_FAILURE);
    }

    
    // 1.注释   2.正确的配置项 =    3.去掉开头的多余的空格 
    while(!feof(pf))
    {
        char buf[512] = {0};
        fgets(buf, 512, pf);

        // 去掉字符串前面多余的空格
        std::string read_buf(buf);
        Trim(read_buf);

        // 判断#的注释
        if (read_buf[0] == '#' || read_buf.empty())
        {
            continue;
        }

        // 解析配置项
        int idx = read_buf.find('=');
        if (idx == -1)
        {
            // 配置项不合法
            continue;
        }

        std::string key;
        std::string value;
        key = read_buf.substr(0, idx);
        Trim(key);

        int endidx = read_buf.find('\n', idx);
        value = read_buf.substr(idx+1, endidx-idx-1);
        Trim(value);
        m_configMap.insert({key, value});
    }

    fclose(pf);
    
    // 2. 通过nacos来加载服务发现中心的基本配置
    std::string nacosserviceIp = m_configMap["nacosserviceIp"];
    std::string nacosservicePort = m_configMap["nacosservicePort"];
    std::string isclient = m_configMap["isclient"];
    MpNacosService mcf(nacosserviceIp, nacosservicePort, isclient.empty()? 0:1);
    
    std::string mongoSocket = mcf.GetConfig("MongoSocket");
    std::string localservername = mcf.GetConfig("localservername");
    m_configMap.insert({"MongoSocket", mongoSocket});
    m_configMap.insert({"localservername", localservername});
}


// 查询配置信息
std::string MprpcConfig::Load(const std::string& key)
{
    auto it = m_configMap.find(key);
    if (it == m_configMap.end())
    {
        return "";
    }
    return it->second;
}

// 去掉字符串前后的空格
void MprpcConfig::Trim(std::string &src_buf)
{
    int idx = src_buf.find_first_not_of(' ');
    if (idx != -1)
    {
        // 说明字符串前面有空格
        src_buf = src_buf.substr(idx, src_buf.size()-idx);
    }
    // 去掉字符串后面多余的空格
    idx = src_buf.find_last_not_of(' ');
    if (idx != -1)
    {
        // 说明字符串后面有空格
        src_buf = src_buf.substr(0, idx+1);
    }
}