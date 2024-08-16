#include "mprpcconfig.h"
#include <iostream>
#include <string>

// 负责解析加载配置文件
void MprpcConfig::LoadConfigFile(const char *config_file)
{
    FILE *pf=fopen(config_file, "r");    // "r"表示读文件
    if(nullptr==pf){
        std::cout<<config_file<<"is not exist!"<<std::endl;
        exit(EXIT_FAILURE);
    }

    // 1.注释 2.正确的配置项 3.去掉开头多余的空格
    while(!feof(pf)){
        char buf[512]={0};
        fgets(buf, 512, pf);

        // 去掉字符串前后多余的空格
        std::string read_buf(buf);
        Trim(read_buf);

        // 判断#的注释
        if(read_buf[0]=='#'||read_buf.empty()){
            continue;
        }

        // 解析配置项
        int idx=read_buf.find('=');
        if(idx==-1){
            // 配置项不合法
            continue;
        }
        std::string key;
        std::string value;
        key=read_buf.substr(0, idx);
        Trim(key);    // 防止rpcserver   = 127.0.0.1这种类型出现
        // rpcserverip=127.0.0.1\n
        int endidx=read_buf.find('\n', idx);    // 不做这一步读出的conf会有空行
        value=read_buf.substr(idx+1, endidx-idx-1);
        Trim(value);
        m_configMap.insert({key, value});
    }
}
// 查询配置项信息
std::string MprpcConfig::Load(const std::string &key)
{
    // 不要用m_configMap[key]不要用中括号 这样如果没查找到会向map中增加一对新的 正常应该返回空字符串
    auto it=m_configMap.find(key);
    if(it==m_configMap.end()){
        return "";
    }
    return it->second;
}

// 去掉字符串前后的空格
void MprpcConfig::Trim(std::string &src_buf){
    int idx=src_buf.find_first_not_of(' ');    // 找第一个不是‘ ’的字符 找到返回其下标 未找到返回-1
    if(idx!=-1){
        // 说明字符串前面有空格 把空格剔除
        src_buf=src_buf.substr(idx, src_buf.size()-idx);    //substr的第二个参数是要截取字符串的长度
    }
    // 去掉字符串后面多余的空格
    idx=src_buf.find_last_not_of(' ');      // find_last 是从后往前找 find和find_first是从前往后找
    if(idx!=-1){
        src_buf=src_buf.substr(0, idx+1);
    }
}