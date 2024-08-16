#include "mprpcapplication.h"
#include <iostream>
#include <unistd.h>

// 类外进行静态成员变量初始化不用带static
MprpcConfig MprpcApplication::m_config;

void ShowArgHelp(){
    std::cout<<"format: command -i <configfile>"<<std::endl;
}
// 类外实现静态方法不用带static
void MprpcApplication::Init(int argc, char **argv){
    if(argc<2){ // IP+Port 两个参数 如果argc<2 说明没有传入任何参数
        ShowArgHelp();  // 输出错误信息
        exit(EXIT_FAILURE);
    }
    int c=0;
    std::string config_file;
    while((c=getopt(argc, argv, "i:"))!=-1){
        switch(c){
            case 'i':
                config_file=optarg;
                break;
            case '?':
                ShowArgHelp();
                exit(EXIT_FAILURE);
            case ':':
                ShowArgHelp();
                exit(EXIT_FAILURE);
            default:
                break;
        }
    }
    // 开始加载配置文件 rpcserver_ip   rpvserver_port   zookeeper_ip   zookeeper_port
    m_config.LoadConfigFile(config_file.c_str());
    std::cout<<"rpcserviceip:"<<m_config.Load("rpcserverip")<<std::endl;
    std::cout<<"rpcserviceport:"<<m_config.Load("rpcserviceport")<<std::endl;
    std::cout<<"zookeeperip:"<<m_config.Load("zookeeperip")<<std::endl;
    std::cout<<"zookeeperport:"<<m_config.Load("zookeeperport")<<std::endl;

}
MprpcApplication& MprpcApplication::GetInstance(){  // 定义唯一的一个实例
    static MprpcApplication app;
    return app;
}

MprpcConfig& MprpcApplication::GetConfig(){
    return m_config;
}