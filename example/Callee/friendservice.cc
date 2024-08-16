#include <iostream>
#include <string>
#include "friend.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"
#include <vector>
#include "logger.h"

class FriendService : public fixbug::FriendServiceRpc{
public:
    // 本地业务
    std::vector<std::string> GetFriendsList(uint32_t userid){
        std::cout<<"do GetFriendsList service!"<<userid<<std::endl;
        std::vector<std::string> vec;
        vec.push_back("gao yang");
        vec.push_back("liu shuo");
        vec.push_back("wang hong");
        return vec;
    }

    void GetFriendsList(::google::protobuf::RpcController* controller,
                       const ::fixbug::GetFriendsListRequest* request,
                       ::fixbug::GetFriendsListResponse* response,
                       ::google::protobuf::Closure* done)
    {
        uint32_t userid=request->userid();
        // 调用本地业务
        std::vector<std::string> friendslist = GetFriendsList(userid);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg("");
        for(std::string &name : friendslist){
            std::string *p=response->add_friends();
            *p=name;
        }

        done->Run();
    }
};

int main(int argc, char **argv){
    // 调用框架的初始化操作 需要读配置文件 所以有两个参数
    MprpcApplication::Init(argc, argv);

    // provider是一个rpc网络服务对象，把UserService对象发布到rpc节点上
    RpcProvider provider;    // 负责数据的序列化反序列化、网络的数据收发
    provider.NotifyService(new FriendService());  // NotifyService记录了FriendService该对象中的方法 供框架调用

    // 启动一个rpc服务发布节点 Run以后、进程进入阻塞状态，等待远程的rpc调用请求
    provider.Run();
    return 0;
}