# 将userservice.cc user.pb.cc加入到SRC_LIST变量
# set(SRC_LIST calluserservice.cc ../user.pb.cc)
set(SRC_LIST callfriendservice.cc ../friend.pb.cc)

# 由SRC_LIST输出可执行文件provider
add_executable(consumer ${SRC_LIST})

# 链接需要的库文件
target_link_libraries(consumer mprpc protobuf)