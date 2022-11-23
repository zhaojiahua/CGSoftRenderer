#最低的cmake程序版本
cmake_minimum_required(VERSION 3.12)

#工程名称
project(ComputeGraphic)

#工程支持的C++版本
set(CMAKE_CXX_STANDARD 17)

#将Functions文件夹纳入编译系统
add_subdirectory(Functions)

#搜索所有的cpp文件,加入到SRCS变量中
aux_source_directory(. SRCS)

#工程所有cpp文件编译链接,生成exe
add_executable(softRenderer "Main.cpp")

#将Functions.lib链入softRenderer
target_link_libraries(softRenderer Functions)