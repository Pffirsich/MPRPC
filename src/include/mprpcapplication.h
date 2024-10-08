#pragma once

#include "mprpcconfig.h"
#include "mprpcchannel.h"
#include "mprpccontroller.h"

// mprpc框架的基础类 负责框架的初始化操作 只需要一个框架即可 设计成单例模式
class MprpcApplication{
public:
    static void Init(int argc, char **argv);
    static MprpcApplication& GetInstance();
    static MprpcConfig& GetConfig();
private:
    static MprpcConfig m_config;
    
    MprpcApplication(){}
    MprpcApplication(const MprpcApplication&)=delete;
    MprpcApplication(MprpcApplication&&)=delete;
};