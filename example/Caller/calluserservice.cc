#include <iostream>
#include "mprpcapplication.h"
#include "user.pb.h"
#include "mprpcchannel.h"

int main(int argc, char **argv){
    // 整个程序启动以后，想使用mprpc框架来享用rpc服务调用，一定需要先调用框架的初始化函数(只初始化一次)
    MprpcApplication::Init(argc, argv);

    // 演示调用远程发布的rpc方法Login
    fixbug::UserServiceRpc_Stub stub(new MprpcChannel());
    // rpc方法的请求参数
    fixbug::LoginRequest request;
    request.set_name("zhang san");
    request.set_pwd("123456");
    // rpc方法的响应
    fixbug::LoginResponse response;
    // 发起rpc方法的调用 底层是MprpcChannel中的CallMethod方法的调用 将发起数据序列化 网络发送 等待远程rpc响应的方法结果
    stub.Login(nullptr, &request, &response, nullptr);
    // 一次rpc调用完成，读调用的结果
    if(response.result().errcode()==0){
        // 调用正确
        std::cout<<"rpc login response:"<<response.success()<<std::endl;
    }
    else{
        std::cout<<"rpc login response error:"<<response.result().errmsg()<<std::endl;
    }
    
    fixbug::RegisterRequest req;
    req.set_id(2000);
    req.set_name("mprpc");
    req.set_pwd("666666");
    fixbug::RegisterResponse res;

    stub.Register(nullptr, &req, &res, nullptr);
    
    if(res.result().errcode()==0){
        // 调用正确
        std::cout<<"rpc register response:"<<res.success()<<std::endl;
    }
    else{
        std::cout<<"rpc register response error:"<<res.result().errmsg()<<std::endl;
    }

    return 0;
}