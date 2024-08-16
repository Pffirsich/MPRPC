# 将.当前目录下的所有源文件的名字保存在SRC_LIST变量中
# aux_source_directory(. SRC_LIST)


set(SRC_LIST mprpcapplication.cc mprpcconfig.cc 
             rpcheader.pb.cc rpcprovider.cc 
             mprpcchannel.cc mprpccontroller.cc
             logger.cc zookeeperutil.cc)

# 指定从一组源文件SRC_LIST编译出一个库文件且命名为mprpc
add_library(mprpc ${SRC_LIST})

# 链接需要的库文件
target_link_libraries(mprpc muduo_net muduo_base pthread zookeeper_mt)