#include <iostream>
#include <string>
#include "user.pb.h"
#include "mprpcapplication.h"
#include "rpcprovider.h"


/*
UserService原来是一个本地服务，提供了两个进程内的本地方法，Login和FriendLists
*/
// UserServiceRpc使用在rpc服务发布端(rpc服务提供者)
class UserService : public fixbug::UserServiceRpc{    // 如果不写fixbug:: 找不到UserServiceRpc
public:
    // 本地业务
    bool Login(std::string name, std::string pwd){
        std::cout<<"doing local service: Login"<<std::endl;
        std::cout<<"name:"<<name<<"pwd:"<<pwd<<std::endl;
        return true;
    }
    bool Register(uint32_t id, std::string name, std::string pwd){
        std::cout<<"doing local service: Register"<<std::endl;
        std::cout<<"id:"<<id<<"name:"<<name<<"pwd:"<<pwd<<std::endl;
        return true;
    }

    // 重写UserServiceRpc基类的虚函数  下面这些方法都是框架直接调用的
    /*
    rpc request先被rpc框架接收 rpc框架根据请求中的方法名等信息确定到是Login方法 然后将该方法上报给Login
    接收到请求后 Login从request中拿出数据做本地业务 将结果放入response中
    做done回调 把返回值塞给框架 由框架对其进行序列化和反序列化 通过框架将其返回回去
    */
   // 调用本地业务
    void Login(::google::protobuf::RpcController* controller,
                       const ::fixbug::LoginRequest* request,  
                       ::fixbug::LoginResponse* response,       
                       ::google::protobuf::Closure* done)
    {
        // 框架给业务上报了请求参数LoginRequest，应用获取相应数据做本地业务
        std::string name = request->name();
        std::string pwd = request->pwd();

        bool login_result = Login(name, pwd);    // 做本地业务

        // 把响应结果写入  包括错误码、错误消息、返回值
        fixbug::ResultCode *code = response->mutable_result();
        code->set_errcode(0);
        code->set_errmsg("");
        response->set_success(login_result);

        // 执行回调操作  执行响应对象数据的序列化和网络发送(都是由框架来完成的)
        done->Run();
    }

    void Register(::google::protobuf::RpcController* controller,
                       const ::fixbug::RegisterRequest* request,
                       ::fixbug::RegisterResponse* response,
                       ::google::protobuf::Closure* done)
    {
        uint32_t id=request->id();
        std::string name=request->name();
        std::string pwd=request->pwd();

        bool ret=Register(id, name, pwd);

        response->mutable_result()->set_errcode(0);
        response->mutable_result()->set_errmsg(0);
        response->set_success(ret);

        done->Run();
    }
};

int main(int argc, char **argv){
    // 调用框架的初始化操作 需要读配置文件 所以有两个参数
    MprpcApplication::Init(argc, argv);

    // provider是一个rpc网络服务对象，把UserService对象发布到rpc节点上
    RpcProvider provider;    // 负责数据的序列化反序列化、网络的数据收发
    provider.NotifyService(new UserService());  // NotifyService记录了UserService该对象中的方法 供框架调用

    // 启动一个rpc服务发布节点 Run以后、进程进入阻塞状态，等待远程的rpc调用请求
    provider.Run();
    return 0;
}