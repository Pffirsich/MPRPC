# 将userservice.cc user.pb.cc加入到SRC_LIST变量
# set(SRC_LIST userservice.cc ../user.pb.cc)
set(SRC_LIST userservice.cc ../user.pb.cc)
# set(SRC_LIST friendservice.cc ../friend.pb.cc)

# 由SRC_LIST输出可执行文件provider
add_executable(provider ${SRC_LIST})

# 链接需要的库文件
target_link_libraries(provider mprpc protobuf)