#最低的cmake程序版本
cmake_minimum_required(VERSION 3.12)

#工程名称
project(ComputeGraphic)

#工程支持的C++版本
set(CMAKE_CXX_STANDARD 17)

#将assets等文件夹里的内容拷贝到运行目录下
file(GLOB copyResource "./assets" "./ThirdParty/bin/assimp/assimp-vc143-mtd.dll")
file(COPY ${copyResource} DESTINATION ${CMAKE_BINARY_DIR})

#指定include和静态库路径
include_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/include
)
link_directories(
	SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/lib/assimp
)

#将Functions文件夹纳入编译系统
add_subdirectory(Functions)

#将application文件夹纳入编译系统
add_subdirectory(application)

#将math文件夹纳入编译系统
add_subdirectory(math)

#将GPU文件夹纳入编译系统
add_subdirectory(GPU)

#搜索所有的cpp文件,加入到SRCS变量中
aux_source_directory(. SRCS)

#工程所有cpp文件编译链接,生成exe
add_executable(softRenderer "Main.cpp" "global/base.cpp")
add_executable(testMath "mathTest.cpp")
add_executable(TestTextrue "textrueTest.cpp" "global/base.cpp")
add_executable(TestCamera "cameraTest.cpp" "global/base.cpp")
add_executable(TestModelImport "modelImportTest.cpp" "global/base.cpp")

#将application.lib GPU.lib链入softRenderer
target_link_libraries(softRenderer applicationlib GPUlib mathlib Functions)

#将application.lib GPU.lib链入TestTextrue
target_link_libraries(TestTextrue applicationlib GPUlib mathlib Functions)

#将application.lib GPU.lib链入TestCamera
target_link_libraries(TestCamera applicationlib GPUlib mathlib Functions)

#将application.lib GPU.lib assimp-vc143.lib链入TestModelImport
target_link_libraries(TestModelImport applicationlib GPUlib mathlib Functions assimp-vc143-mtd.lib)