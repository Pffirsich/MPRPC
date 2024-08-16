#include "zookeeperutil.h"
#include "mprpcapplication.h"
#include <semaphore.h>
#include <iostream>

// 全局的watcher观察器  zkserver给zkclient的通知
void global_watcher(zhandle_t *zh, int type, int state, const char *path, void *watcherCtx){
    if(type==ZOO_SESSION_EVENT){  // 回调的消息类型是和会话相关的消息类型
        if(state==ZOO_CONNECTED_STATE){  //zkclient和zkserver连接成功
            sem_t *sem=(sem_t *)zoo_get_context(zh);
            sem_post(sem);  // 给信号量+1
        }
    }
}

ZkClient::ZkClient() : m_zhandle(nullptr){

}

ZkClient::~ZkClient(){
    if(m_zhandle!=nullptr){  // 如果m_zhandle不为空说明该句柄与zkserver已连接 故关闭掉该句柄
        zookeeper_close(m_zhandle);
    }
}

void ZkClient::Start(){
    std::string host = MprpcApplication::GetInstance().GetConfig().Load("zookeeperip");
    std::string port = MprpcApplication::GetInstance().GetConfig().Load("zookeeperport");
    /*因为zookeeper_init的第一个参数有格式要求param host comma separated host:port pairs, each corresponding to a zk
        server. e.g. "127.0.0.1:3000,127.0.0.1:3001,127.0.0.1:3002" 故需要connstr */
    std::string connstr = host+":"+port;

    /*
    zookeeper_mt：多线程版本
    zookeeper的API客户端程序提供了三个线程
    API调用线程
    网络I/O线程 pthread_create poll  以1/3*timeout时间(10s)发送ping消息 作为心跳信号
    watcher回调线程 zkserver给zkclient通知消息 pthread_create
    */
    m_zhandle=zookeeper_init(connstr.c_str(), global_watcher, 30000, nullptr, nullptr, 0);  // 30000是会话超时时间
    if(nullptr==m_zhandle){
        std::cout<<"zookeeper_init error!"<<std::endl;
        exit(EXIT_FAILURE);
    }

    // 创建信号量并初始化
    sem_t sem;
    sem_init(&sem, 0, 0);
    // 将信号量sem添加到句柄m_zhandle上
    zoo_set_context(m_zhandle, &sem);

    sem_wait(&sem);  //一开始sem是0 sem_wait会阻塞等待 等调用了watcher线程后会给sem+1 这时不再阻塞
    std::cout<<"zookeeper_init success!"<<std::endl;
}

// 创建znode节点
void ZkClient::Create(const char *path, const char *data, int datalen, int state){
    char path_buffer[128];
    int bufferlen=sizeof(path_buffer);
    int flag;
    // 先判断path表示的znode节点是否存在 不存在才创建
    flag=zoo_exists(m_zhandle, path, 0, nullptr);
    if(ZNONODE==flag){  // 表示path表示的znode节点不存在
        flag=zoo_create(m_zhandle, path, data, datalen, &ZOO_OPEN_ACL_UNSAFE, state, path_buffer, bufferlen);
        if(flag==ZOK){
            std::cout<<"znode create success... path"<<path<<std::endl;
        }
        else{
            std::cout<<"flag:"<<flag<<std::endl;
            std::cout<<"znode create error... path"<<path<<std::endl;
            exit(EXIT_FAILURE);
        }
    }
}

// 根据指定的path，获取znode节点的值
std::string ZkClient::GetData(const char *path){
    char buffer[64];
    int bufferlen=sizeof(buffer);
    int flag=zoo_get(m_zhandle, path, 0, buffer, &bufferlen, nullptr);
    if(flag!=ZOK){
        std::cout<<"get znode error... path:"<<path<<std::endl;
        return "";
    }
    else{return buffer;}  // 返回ip+port
}