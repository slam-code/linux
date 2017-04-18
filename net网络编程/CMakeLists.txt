# CMake 最低版本号要求
cmake_minimum_required(VERSION 2.8)

# 项目信息
project(alg)

# include路径
include_directories(${PROJECT_SOURCE_DIR}/lib)

# 设置可执行文件生成路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/../bin)


#SET (CMAKE_C_COMPILER  "/usr/bin/clang")
#SET(CMAKE_CXX_COMPILER "clang++")


SET(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS}     -std=c++11 ")#-stdlib=libc++  -lstdc++ -lrt

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS 变量
aux_source_directory(. DIR_SRCS)

# 指定生成目标

add_executable( udp_client udp_client.cpp)
add_executable(udp_server udp_server.cpp)
add_executable(tcp_server tcp_server.cpp)
#add_executable(wy2 wy2.cpp)
