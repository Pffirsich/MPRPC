#include <iostream>
#include "mprpcapplication.h"
#include "friend.pb.h"

int main(int argc, char **argv){
    // 整个程序启动以后，想使用mprpc框架来享用rpc服务调用，一定需要先调用框架的初始化函数(只初始化一次)
    MprpcApplication::Init(argc, argv);

    // 演示调用远程发布的rpc方法Login
    fixbug::FriendServiceRpc_Stub stub(new MprpcChannel());
    // rpc方法的请求参数
    fixbug::GetFriendsListRequest request;
    request.set_userid(1000);
    // rpc方法的响应
    fixbug::GetFriendsListResponse response;
    // 发起rpc方法的调用 底层是MprpcChannel中的CallMethod方法的调用 将发起数据序列化 网络发送 等待远程rpc响应的方法结果
    MprpcController controller;  // 用以知道调用过程中出现的一些状态信息
    stub.GetFriendsList(&controller, &request, &response, nullptr);
    // 一次rpc调用完成，读调用的结果
    if(controller.Failed()){  // rpc调用过程出错
        std::cout<<controller.ErrorText()<<std::endl;  // 打印错误信息
    }
    else{
        if(response.result().errcode()==0){
            // 调用正确
            std::cout<<"rpc getfriendslist response:"<<std::endl;
            int size=response.friends_size();
            for(int i=0; i<size; i++){
                std::cout<<"index:"<<(i+1)<<" name: "<<response.friends(i)<<std::endl;
            }
        }
        else{
            std::cout<<"rpc getfriendslist response error:"<<response.result().errmsg()<<std::endl;
        }
    }
    
    return 0;
}